#ifndef MATRIX_H
#define MATRIX_H

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
    void spvm(const double* localProductArray, int numGhostEntries, const int* columnToGhostIndex, const double* ghostVector, int size, int rank, double* resultArray);
};

#endif
