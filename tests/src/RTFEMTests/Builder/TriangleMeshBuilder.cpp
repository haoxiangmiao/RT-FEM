#include <RTFEMTests/Builder/TriangleMeshBuilder.h>

#include <RTFEM/FEM/Meshing/TriangleMesh.h>
#include <RTFEM/Memory/UniquePointer.h>

std::unique_ptr<rtfem::TriangleMeshIndexed<float>>
TriangleMeshBuilder::BuildCube(){
    auto triangle_mesh = rtfem::make_unique<rtfem::TriangleMeshIndexed<float>>();
    triangle_mesh->points = {
            Eigen::Vector3<float>{0, 0, 0}, // 0
            Eigen::Vector3<float>{0, 0, 1}, // 1
            Eigen::Vector3<float>{1, 0, 0}, // 2
            Eigen::Vector3<float>{1, 0, 1}, // 3

            Eigen::Vector3<float>{0, 1, 0}, // 4
            Eigen::Vector3<float>{0, 1, 1}, // 5
            Eigen::Vector3<float>{1, 1, 0}, // 6
            Eigen::Vector3<float>{1, 1, 1}, // 7
    };

    triangle_mesh->triangles = {
            // Bottom
            rtfem::TriangleIndices{0, 1, 2},
            rtfem::TriangleIndices{1, 2, 3},
            // Top
            rtfem::TriangleIndices{4, 5, 6},
            rtfem::TriangleIndices{5, 6, 7},
            // Left
            rtfem::TriangleIndices{5, 4, 0},
            rtfem::TriangleIndices{5, 1, 0},
            // Right
            rtfem::TriangleIndices{6, 7, 2},
            rtfem::TriangleIndices{7, 3, 2},
            // Back
            rtfem::TriangleIndices{4, 6, 0},
            rtfem::TriangleIndices{6, 2, 0},
            // Front
            rtfem::TriangleIndices{5, 1, 3},
            rtfem::TriangleIndices{5, 7, 3},

    };

    return triangle_mesh;
}