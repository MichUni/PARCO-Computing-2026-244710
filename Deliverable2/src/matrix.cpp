#include "matrix.h"

#include <iostream>
#include <algorithm>
#include <string>

matrix::matrix(int localNumRows, int localNumValues):numRows(localNumRows), numValues(localNumValues) {
    aRows = new int[numRows + 1];
    aCols = new int[numValues];
    vals = new double[numValues];

    std::fill_n(aRows, numRows + 1, 0);
}

matrix::~matrix() {
    delete[] aRows;
    delete[] aCols;
    delete[] vals;
}

void matrix::coo_to_csr(int* rows, int* cols, double* values) {
	for(int i = 0;i < numValues;i++) {
		if(rows[i] < 0 || rows[i] > numRows) {
            std::cerr << "error in rows number " << rows[i] << std::endl;
        }
        
		aRows[rows[i] + 1]++;
		aCols[i] = cols[i];
		vals[i] = values[i];
	}
		
	for(int i = 1;i < numRows;i++)
		aRows[i + 1] += aRows[i];
}

void matrix::spmv(const double* localProductArray, int numGhostEntries, const int* columnToGhostIndex, const double* ghostVector, int size, int rank, double* resultArray) {
	for(int i = 0;i < numRows;i++) {
        int startIndex = aRows[i];
        int endIndex = aRows[i + 1];
        
        for(int j = startIndex;j < endIndex;j++) {
        	int col = aCols[j];
        	
        	if(col % size == rank) {
        		resultArray[i] += vals[j] * localProductArray[col / size];
			} else {
				if(columnToGhostIndex[col] == -1) {
					std::cerr << "error 1" << std::endl;
					continue;
				}
				
				resultArray[i] += vals[j] * ghostVector[columnToGhostIndex[col]];
			}
        }
    }
}
