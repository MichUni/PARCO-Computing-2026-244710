#ifndef MATRIX_H
#define MATRIX_H

#include <string>

class matrix {
    int numRows;
    int numCols;

    int* aRows;
    int* aCols;
    double* values;

    double* productArr;
    double* resultArr;
public:
    matrix(const std::string fileName);
    ~matrix();

    void sequentialProduct();
    void parallelProduct(int numOfThreads);
};

#endif
