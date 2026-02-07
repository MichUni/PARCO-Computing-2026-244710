#include "matrix.h"

#include <iostream>
#include <string>

matrix::matrix(int localNumRows, int localNumValues):numRows(localNumRows), numValues(localNumValues) {
    aRows = new int[numRows + 1];
    aCols = new int[numValues];
    vals = new double[numValues];

    std::fill(aRows, aRows + numRows + 1, 0);
}

matrix::~matrix() {
    delete[] aRows;
    delete[] aCols;
    delete[] values;
}

void matrix::coo_to_csr(int* rows, int* cols, double* values) {
	for(int i = 0;i < numValues + 1;i++) {
		aRows[rows[i] + 1]++;
		aCols[i] = cols[i];
		vals[i] = values[i];
	}
		
	for(int i = 1;i < numRows;i++)
		aRows[i + 1] += aRows[i];
}

void matrix::print(int* rows, int* cols, double* values, int rank) {
	std::string temp = "|";

	for(int i = 0;i < numValues;i++)
    	temp = temp + " " + std::to_string(rows[i]) + " - " + std::to_string(cols[i]) + " - " + std::to_string(values[i]) + " |";
  
	temp = temp + "\n";
	std::cout << temp;
}

void matrix::spmv(double* x, double* y) {
	for(int i = 0;i < numRows;i++) {
        int startIndex = aRows[i];
        int endIndex = aRows[i + 1];
        
        for(int j = startIndex;j < endIndex;j++) {
            resultArr[i] += values[j] * productArr[aCols[j]];
        }
    }
}
