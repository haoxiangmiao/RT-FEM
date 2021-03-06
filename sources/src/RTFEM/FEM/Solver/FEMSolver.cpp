#include "RTFEM/FEM/Solver/FEMSolver.h"

#include <Eigen/LU>
#include <Eigen/Core>

namespace rtfem {

template<class T>
FEMSolver<T>::FEMSolver(FEMModel<T>* fem_model) :
    fem_model_(fem_model),
    type_(FEMSolverType::GPU){}

template<class T>
Eigen::Vector<T, Eigen::Dynamic>
FEMSolver<T>::SolveSystemOfEquations(
    const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& A,
    const Eigen::Vector<T, Eigen::Dynamic>& b){
    return A.fullPivLu().solve(b);
}

template
class FEMSolver<double>;
template
class FEMSolver<float>;

}