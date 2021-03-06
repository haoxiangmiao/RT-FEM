#ifndef PROJECT_SPARSEMATRIXCSR_H
#define PROJECT_SPARSEMATRIXCSR_H

#include<vector>

namespace rtfem {

enum class MatrixType{
    General, Symmetric
};

/**
 * m x n
 * @tparam T
 */
template<class T>
class SparseMatrixCSR {
public:
    SparseMatrixCSR(std::vector<T> values,
                    std::vector<int> row_extents,
                    std::vector<int> columns_indices,
                    unsigned int m,
                    unsigned int n,
                    MatrixType type);
    ~SparseMatrixCSR() = default;

    const std::vector<T>& values() const {return values_;}
    const std::vector<int>& row_extents() const {return row_extents_;}
    const std::vector<int>& columns_indices() const {return
                columns_indices_;}
    unsigned int m() const {return m_;}
    unsigned int n() const {return n_;}

    MatrixType type() const { return type_; }
private:
    std::vector<T> values_;
    std::vector<int> row_extents_;
    std::vector<int> columns_indices_;

    unsigned int m_;
    unsigned int n_;

    MatrixType type_;
};
}


#endif //PROJECT_SPARSEMATRIXCSR_H
