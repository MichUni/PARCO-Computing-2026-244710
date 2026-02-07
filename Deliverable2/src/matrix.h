#ifndef MATRIX_H
#define MATRIX_H

#include <string>

class matrix {
public:
    int numRows;
    int numValues;

    int* aRows;
    int* aCols;
    double* vals;
    
    matrix(int localRows, int localNnz);
    ~matrix();

    void coo_to_csr(int* rows, int* cols, double* values);
    void spmv(const double* &localProductArray, int numGhostEntries, const int* &ghostColumns, const double* &ghostEntries, int size, int rank, double* resultArray);
};

#endif
