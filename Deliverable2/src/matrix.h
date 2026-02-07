#ifndef MATRIX_H
#define MATRIX_H

#include <string>

class matrix {
    int numRows;
    int numValues;

    int* aRows;
    int* aCols;
    double* vals;
public:
    matrix(int localRows, int localNnz);
    ~matrix();

    void coo_to_csr(int* rows, int* cols, double* values);
    void print(int* rows, int* cols, double* values, int rank);
    void spmv(double* x, double* y);
};

#endif
