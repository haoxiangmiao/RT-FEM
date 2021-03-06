#ifndef PROJECT_VERTEX_H
#define PROJECT_VERTEX_H

#include <RTFEM/DataTypes.h>

namespace rtfem {

template<class T>
class Vertex {
public:
    Vertex(unsigned int id, const Eigen::Vector3<T> &coordinates);

    ~Vertex() = default;

    unsigned int id() const { return id_; }
    const Eigen::Vector3<T> &coordinates() const { return coordinates_; }
    void coordinates(const Eigen::Vector3<T> & coordinates) {
        coordinates_ = coordinates; }

    T x() const { return coordinates_(0); }
    T y() const { return coordinates_(1); }
    T z() const { return coordinates_(2); }

private:
    /**
     * ID is assumed to be continues integer numbers [0,N]
     */
    unsigned int id_;

    Eigen::Vector3<T> coordinates_;
};

}

#endif //PROJECT_VERTEX_H
