#ifndef PROJECT_FEMDYNAMICASSEMBLER_H
#define PROJECT_FEMDYNAMICASSEMBLER_H

#include <RTFEM/FEM/Solver/FEMGlobalAssembler.h>

namespace rtfem {

template<class T>
class FEMGlobalDynamicAssembler : public FEMGlobalAssembler<T>{
public:
    FEMGlobalDynamicAssembler() = default;
    ~FEMGlobalDynamicAssembler() = default;

protected:
    virtual void ComputeAssemblerData(
            FEMGlobalAssemblerData<T> &fem_assembler_data,
            FEMModel<T> &fem_model,
            Eigen::Matrix<T, CONSTITUTIVE_MATRIX_N, CONSTITUTIVE_MATRIX_N> &
            constitutive_matrix_C,
            bool force_only) override;

    virtual void ComputeAssemblerDataIteration(
            FEMGlobalAssemblerData<T> &fem_assembler_data,
            const FiniteElementSolverData<T>& finite_element_solver_data,
            const FEMModel<T> &fem_model,
            const Eigen::Matrix<T, CONSTITUTIVE_MATRIX_N, CONSTITUTIVE_MATRIX_N>
            &constitutive_matrix_C,
            const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>&
            boolean_assembly_matrix_A,
            bool force_only) override;

    virtual void ApplyBoundaryConditionsToFEM(
        FEMGlobalAssemblerData<T> &assembler_data,
        const BoundaryConditionContainer<T> &boundary_conditions) override;
private:
    void
    ComputePartialGlobalMassMatrix(
            T density, T volume,
            const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>&
            boolean_assembly_matrix_A,
            Eigen::DiagonalMatrix<T, Eigen::Dynamic>& global_mass);

    Eigen::DiagonalMatrix<T, Eigen::Dynamic>
    ComputeLocalMassMatrix(T density, T volume);

    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>
    ComputeGlobalDampingMatrix(
        const Eigen::DiagonalMatrix<T, Eigen::Dynamic>&
        global_mass_matrix,
        const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>&
        global_stiffness_matrix,
        T damping_mass,
        T damping_stiffness);

    void AssembleGlobalPositionVector(
        const std::vector<std::shared_ptr<Vertex<T>>>& vertices,
        Eigen::Vector<T, Eigen::Dynamic>& global_position);
};
}


#endif //PROJECT_FEMDYNAMICASSEMBLER_H

