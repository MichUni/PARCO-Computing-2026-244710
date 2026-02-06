#include "matrix.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

matrix::matrix(int localRows, int localNnz):numRows(localRows), nnz(localNnz) {
    aRows = new int[numRows + 1];
    aCols = new int[nnz];
    values = new double[nnz];

    std::fill(aRows, aRows + numRows + 1, 0);
}

matrix::~matrix() {
    delete[] aRows;
    delete[] aCols;
    delete[] values;
}

void matrix::coo_to_csr() {
	for(int i = 0;i < numRows;i++) {
        int startIndex = aRows[i];
        int endIndex = aRows[i + 1];
        
        for(int j = startIndex;j < endIndex;j++) {
            resultArr[i] += values[j] * productArr[aCols[j]];
        }
    }
}

void matrix::parallelProduct(int numOfThreads) {
    #pragma omp parallel for schedule(runtime) num_threads(numOfThreads)
        for(int i = 0;i < numRows;i++) {
            double sum = 0;
            
            int startIndex = aRows[i];
            int endIndex = aRows[i + 1];

            for(int j = startIndex;j < endIndex;j++) {
                sum += values[j] * productArr[aCols[j]];
            }
            
            resultArr[i] = sum;
        }
}
