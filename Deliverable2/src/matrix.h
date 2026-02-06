#ifndef MATRIX_H
#define MATRIX_H

#include <string>

class matrix {
    int numRows;
    int numNnz;

    int* aRows;
    int* aCols;
    double* values;
public:
    matrix(int localRows, int localNnz);
    ~matrix();

    void coo_to_csr(const std::vector<int>& rows, const std::vector<int>& cols, const std::vector<double>& vals);
    void spmv(const double* x, double* y);
};

#endif
