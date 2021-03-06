#ifndef PROJECT_GLOBALSTIFFNESSASSEMBLER_H
#define PROJECT_GLOBALSTIFFNESSASSEMBLER_H

#include <RTFEM/DataTypes.h>

#include <memory>
#include <vector>
#include <RTFEM/Timer.h>

namespace rtfem {

template<class T>
class FEMModel;

template<class T>
class FiniteElement;

template<class T>
class Vertex;

template<class T>
class FiniteElementSolver;

template<class T>
struct FEMGeometry;

template<class T>
class BoundaryConditionContainer;

template<class T>
struct FiniteElementSolverData;

enum class FiniteElementType;

template<class T>
struct FEMGlobalAssemblerData {
    FEMGlobalAssemblerData(unsigned int global_dof_count) :
        global_mass_diagonal(
            Eigen::Vector<
                T,
                Eigen::Dynamic>::
            Zero(global_dof_count)),
        global_damping(
            Eigen::Matrix<
                    T,
                    Eigen::Dynamic,
                    Eigen::Dynamic>::
            Zero(global_dof_count, global_dof_count)),
        global_stiffness(
            Eigen::Matrix<
                T,
                Eigen::Dynamic,
                Eigen::Dynamic>::
            Zero(global_dof_count, global_dof_count)),
        global_force(
            Eigen::Vector<
                T,
                Eigen::Dynamic>::
            Zero(global_dof_count)),
        global_position(
            Eigen::Vector<
                T,
                Eigen::Dynamic>::
            Zero(global_dof_count)){}

    Eigen::DiagonalMatrix<T, Eigen::Dynamic> global_mass_diagonal;
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> global_damping;
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> global_stiffness;
    Eigen::Vector<T, Eigen::Dynamic> global_force;
    // TODO: This is wasting memory! duplication of FemGeometry::Vertex
    Eigen::Vector<T, Eigen::Dynamic> global_position;
};

class FEMGlobalAssemblerTimer : public Timer {
public:
    double TotalTime() const {
        return finite_element_solver_time +
               boolean_assembly_matrix_time +
               partial_global_stiffness_time +
               partial_global_mass_time +
               partial_global_damping_time +
               partial_global_force_time;
    }

    void ComputeAverage(int n) {
        finite_element_solver_time /= n;
        boolean_assembly_matrix_time /= n;
        partial_global_stiffness_time /= n;
        partial_global_force_time /= n;
    }

    double finite_element_solver_time = 0;
    double boolean_assembly_matrix_time = 0;
    double partial_global_stiffness_time = 0;

    double partial_global_mass_time = 0;
    double partial_global_mass_time_transpose = 0;
    double partial_global_mass_time_cuda1 = 0;
    double partial_global_mass_time_cuda2 = 0;

    double partial_global_damping_time = 0;
    double partial_global_force_time = 0;
};

constexpr int CONSTITUTIVE_MATRIX_N = 6;
constexpr unsigned int DIMENSION_COUNT = 3;

/**
 * Computes and Assembles Global Stiffness Matrix and Global Force Vector.
 *
 * Local Stiffness Matrix (k) is the stiffness of each element [3Ne x 3Ne]
 * e.g. for Tetrahedron (Ne = 4) thus: (dim = [12 x 12])
 *
 * Global Stiffness Matrix (K) is the stiffness of entire FEM Model [3N x 3N]
 * e.g. For 9 vertices (dim = [27 x 27])
 *
 * Partial Global Stiffness Matrix (Ke) is the matrix of dimension equal to Global Stiffness
 * but filled only with Local Stiffness data.
 *
 * Partial Global Force Vector (Qe) [3N x 1]
 * Global Force Vector (Q) [3N x 1]
 */
template<class T>
class FEMGlobalAssembler {
public:
    FEMGlobalAssembler() = default;
    virtual ~FEMGlobalAssembler() = default;

    const FEMGlobalAssemblerTimer& timer(){return timer_;}

    /**
     * Computes Global Stiffness Matrix (K) and Global Force Vector (Q).
     *
     *      1) Computes Constitutive Matrix (C)
     *      2) Computes Geometry Matrix (B) for each Finite Element
     *      3) Computes Local Stiffness (k) for each Finite Element
     *          - Using Constitutive Matrix and Geometry Matrix.
     *      4) Assembles all Local Stiffness matrices into Global Stiffness Matrix (K)
     *
     * @param fem_model
     * @return
     */
    FEMGlobalAssemblerData<T> Compute(FEMModel<T> &fem_model,
                                      bool force_only = false);

    void ApplyBoundaryConditions(
        Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& matrix,
        Eigen::Vector<T, Eigen::Dynamic>& vector,
        const BoundaryConditionContainer<T> &boundary_conditions);
    void ApplyBoundaryConditionsMatrix(
            Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& matrix,
            const BoundaryConditionContainer<T> &boundary_conditions);
    void ApplyBoundaryConditionsMatrix(
            Eigen::DiagonalMatrix<T, Eigen::Dynamic>& matrix,
            const BoundaryConditionContainer<T> &boundary_conditions);
    void ApplyBoundaryConditionsVector(
            Eigen::Vector<T, Eigen::Dynamic>& vector,
            const BoundaryConditionContainer<T> &boundary_conditions);
protected:
    /**
     * Iterates through every finite element and assembles data into
     * Global matrices
     *
     * @param fem_assembler_data
     * @param fem_geometry
     * @param constitutive_matrix_C
     */
    virtual void ComputeAssemblerData(
        FEMGlobalAssemblerData<T> &fem_assembler_data,
        FEMModel<T> &fem_model,
        Eigen::Matrix<T, CONSTITUTIVE_MATRIX_N, CONSTITUTIVE_MATRIX_N> &
        constitutive_matrix_C,
        bool force_only);

    virtual void ComputeAssemblerDataIteration(
            FEMGlobalAssemblerData<T> &fem_assembler_data,
            const FiniteElementSolverData<T>& finite_element_solver_data,
            const FEMModel<T> &fem_model,
            const Eigen::Matrix<T, CONSTITUTIVE_MATRIX_N,CONSTITUTIVE_MATRIX_N> &
            constitutive_matrix_C,
            const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>&
            boolean_assembly_matrix_A,
            bool force_only);

    /**
     * Applies Boundary Conditions to the GlobalStiffness and
     * GlobalForce Vector
     * @param assembler_data
     * @param boundary_conditions
     */
    virtual void ApplyBoundaryConditionsToFEM(
        FEMGlobalAssemblerData<T> &assembler_data,
        const BoundaryConditionContainer<T> &boundary_conditions);

    FEMGlobalAssemblerTimer timer_;
private:
    /**
     * Computes Isotropic Constitutive Matrix (C).
     * Used to compute Local Stiffness.
     *
     * In case of Linear Elasticity, Constitutive Matrix is constant.
     *
     * Isotropy allows Constitutive Matrix to remain unchanged
     * after orthogonal transformations
     *
     * @param fem_model
     * @return
     */
    Eigen::Matrix<T, CONSTITUTIVE_MATRIX_N, CONSTITUTIVE_MATRIX_N>
    ComputeConstitutiveMatrix(const FEMModel<T>& fem_model);

    /**
     * Computes Geometry Matrix (B).
     * Used to compute Local Stiffness
     *
     * @param type
     * @return
     */
    std::unique_ptr<FiniteElementSolver<T>> GetFiniteElementSolver(const FiniteElementType &type);

    /**
     * Computes The Boolean Assembly Matrix (A) [3Ne x 3N].
     *
     * Maps Local Stiffness to Global Stiffness.
     *
     * @param finite_element
     * @param vertices
     * @return
     */
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>
    ComputeBooleanAssemblyMatrix(
        const std::shared_ptr<FiniteElement<T>> finite_element,
        const std::vector<std::shared_ptr<Vertex<T>>> &vertices);

    /**
     * Computes Partial Global Stiffness Matrix.
     * Uses Boolean Assembly Matrix to map a single Local Stiffness to Global Stiffness.
     *
     * @param boolean_assembly_matrix_A
     * @param local_stiffness_k
     * @return
     */
    void
    ComputePartialGlobalStiffnessMatrix(
        const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &geometry_matrix,
        const Eigen::Matrix<T,
                            CONSTITUTIVE_MATRIX_N,
                            CONSTITUTIVE_MATRIX_N> &constitutive_matrix_C,
        const Eigen::Matrix<T,
                            Eigen::Dynamic,
                            Eigen::Dynamic> &boolean_assembly_matrix_A,
        T volume,
        Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& global_stiffness);

    void
    ComputePartialGlobalStiffnessMatrix2(
            const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &geometry_matrix,
            const Eigen::Matrix<T,
                    CONSTITUTIVE_MATRIX_N,
                    CONSTITUTIVE_MATRIX_N> &constitutive_matrix_C,
            const FiniteElement<T>& finite_element,
            T volume,
            Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& global_stiffness);

    /**
     * Computes Local Stiffness (k) of a given element.
     *
     * @param finite_element
     * @param constitutive_matrix
     * @param volume
     * @return
     */
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>
    ComputeLocalStiffness(
        const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &geometry_matrix,
        const Eigen::Matrix<T,
                            CONSTITUTIVE_MATRIX_N,
                            CONSTITUTIVE_MATRIX_N> &constitutive_matrix,
        T volume);

    /**
     * Computes the Partial Global Force vector (Q)
     * @param force_vector
     * @param boolean_assembly_matrix_A
     * @return
     */
    Eigen::Vector<T, Eigen::Dynamic>
    ComputePartialGlobalForceVector(
        const Eigen::Vector<T, Eigen::Dynamic> &force_vector,
        const Eigen::Matrix<T,
                            Eigen::Dynamic,
                            Eigen::Dynamic> &boolean_assembly_matrix_A);
};
}

#endif //PROJECT_GLOBALSTIFFNESSASSEMBLER_H
