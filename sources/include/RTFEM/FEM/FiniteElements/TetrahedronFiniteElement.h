#ifndef PROJECT_TETRAHEDRONFINITEELEMENT_H
#define PROJECT_TETRAHEDRONFINITEELEMENT_H

#include <RTFEM/FEM/FiniteElement.h>

#include <memory>

namespace rtfem {

template<class T>
class Vertex;

template<class T>
class TetrahedronFiniteElement : public FiniteElement<T> {
public:
    TetrahedronFiniteElement(unsigned int vertex1,
                             unsigned int vertex2,
                             unsigned int vertex3,
                             unsigned int vertex4,
                             unsigned int face1,
                             unsigned int face2,
                             unsigned int face3,
                             unsigned int face4);

    ~TetrahedronFiniteElement() = default;

    unsigned int GetVertexCount() const override;
    unsigned int GetFaceCount() const override;

private:
    const unsigned int vertex_count = 4;
    const unsigned int face_count = 4;
};
}

#endif //PROJECT_TETRAHEDRONFINITEELEMENT_H
