#ifndef MATRIX_H
#define MATRIX_H

#include <string>

class matrix {
    int numRows;
    int numCols;

    int* aRows;
    int* aCols;
    double* values;

public:
    matrix(const std::string fileName);
    ~matrix();

    void sequentialProduct(double* &resultArray, int &arrLength);
    void parallelProduct(double* &resultArray, int &arrLength, int numOfThreads);
};

#endif
