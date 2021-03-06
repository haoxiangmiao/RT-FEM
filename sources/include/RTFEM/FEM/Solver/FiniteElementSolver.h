#ifndef PROJECT_FINITEELEMENTSOLVER_H
#define PROJECT_FINITEELEMENTSOLVER_H

#include <RTFEM/DataTypes.h>

#include <memory>
#include <vector>

namespace rtfem {

template<class T>
struct Material;

template<class T>
class FiniteElement;

template<class T>
class Vertex;

template<class T>
struct TriangleFace;

/**
*  Contains:
*      Volume (V)
*      Geometry Matrix (B)
*      Force vector (p)
*
*  Coordinates:
*      x2 is assumed to point 'up'
*/
template<class T>
struct FiniteElementSolverData {
    FiniteElementSolverData() :
        volume(0) {}
    T volume;

    // Used to compute Global Stiffness
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> geometry_matrix;

    // Used to compute Global Force
    Eigen::Matrix<T, Eigen::Dynamic, 1> force_vector;
};

/**
 * Pressure forces directed along unit normal of i-th face
 * (traction_force_face1, i == 1)
 */
template<class T>
struct TractionForces {
    T force_face1 = 0;
    T force_face2 = 0;
    T force_face3 = 0;
    T force_face4 = 0;
};

enum class FiniteElementSolverStatus{
    OK, ERROR_VOLUME_0, ERROR_VOLUME_NEGATIVE
};

/**
 * Computes FiniteElementSolverData for a given FiniteElement.
 */
template<class T>
class FiniteElementSolver {
public:
    FiniteElementSolver();
    virtual ~FiniteElementSolver() = default;

    virtual FiniteElementSolverData<T> Solve(
        std::shared_ptr<FiniteElement<T>> finite_element,
        const std::vector<std::shared_ptr<Vertex<T>>> &vertices,
        std::vector<TriangleFace<T>> &triangle_faces,
        const Eigen::Vector3<T> &body_force,
        const Material<T>& material) = 0;

    const FiniteElementSolverStatus& GetStatus();
protected:
    FiniteElementSolverStatus status_;
};
}

#endif //PROJECT_FINITEELEMENTSOLVER_H
