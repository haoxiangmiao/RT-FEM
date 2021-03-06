#include "RTFEM/FEM/FiniteElement.h"

namespace rtfem {

template<class T>
FiniteElement<T>::FiniteElement(const FiniteElementType &&type) :
        volume_(0),
        type_(type) {}

template<class T>
void FiniteElement<T>::SetFacesIndices(
    const std::vector<unsigned int> &faces_indices){
    faces_indices_ = faces_indices;
}

template<class T>
void FiniteElement<T>::SetVolume(T volume){
    volume_ = volume;
}

template
class FiniteElement<double>;
template
class FiniteElement<float>;

}
