#ifndef PROJECT_FINITEELEMENT_H
#define PROJECT_FINITEELEMENT_H

#include <RTFEM/FEM/FiniteElements/FiniteElementType.h>
#include <RTFEM/DataTypes.h>

#include <vector>
#include <memory>

namespace rtfem {

template<class T>
class Vertex;

/**
 * Abstract class for Finite Element.
 */
template<class T>
class FiniteElement {
public:
    FiniteElement(const FiniteElementType&& type);
    virtual ~FiniteElement() = default;

    const FiniteElementType& type() const {return type_;}

    const std::vector<unsigned int>& vertices_indices(){return
            vertices_indices_;}

    virtual unsigned int GetVertexCount() const = 0;

protected:
    std::vector<unsigned int> vertices_indices_;
private:
    FiniteElementType type_;
};
}


#endif //PROJECT_FINITEELEMENT_H
