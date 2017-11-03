#include "RTFEMTests/FEM/Solver/Solvers/FEMStaticSolverTest.h"

#include <RTFEM/FEM/Solver/FEMSolver.h>
#include <RTFEM/FEM/Solver/FEMSolvers/FEMStaticSolver.h>
#include <RTFEM/FEM/FEMModel.h>
#include <RTFEM/DataTypes.h>
#include <RTFEMTests/Builder/FEMModelBoxBuilder.h>
#include <RTFEM/Memory/UniquePointer.h>

void FEMStaticSolverTest::SetUp() {
    FEMModelBoxBuilder builder;
    fem_model_ = builder.Create();
}

void FEMStaticSolverTest::TearDown() {
}

TEST_F(FEMStaticSolverTest, FEMSolver_EmptyModel_DisplacemntsAreZero) {
    rtfem::FEMStaticSolver<double> fem_solver;

    auto fem_solver_output = fem_solver.Solve(*fem_model_);

    for(unsigned int i = 0; i < fem_solver_output.displacement.size(); i++){
        EXPECT_EQ(0, fem_solver_output.displacement[i]);
    }
}

TEST_F(FEMStaticSolverTest, FEMSolver_GravityForce_Displacemnts) {
    rtfem::FEMStaticSolver<double> fem_solver;

    fem_model_->SetBodyForce(Eigen::Vector3<double>{
        0, -9.81* 1000000, 0
    });
    auto fem_solver_output = fem_solver.Solve(*fem_model_);

/*
    for(unsigned int i = 0; i < fem_solver_output.displacement.size(); i++){
        EXPECT_EQ(0, fem_solver_output.displacement[i]);
    }*/
}