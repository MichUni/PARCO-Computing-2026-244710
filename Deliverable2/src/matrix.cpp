#include "matrix.h"

#include <iostream>

matrix::matrix(int localNumRows, int localNumValues):numRows(localNumRows), numValues(localNumValues) {
    aRows = new int[numRows + 1];
    aCols = new int[numValues];
    values = new double[numValues];

    std::fill(aRows, aRows + numRows + 1, 0);
}

matrix::~matrix() {
    delete[] aRows;
    delete[] aCols;
    delete[] values;
}

void matrix::coo_to_csr(int* rows, int* cols, double* values) {
	for(int i = 0;i < numRows + 1;i++)
		aRows[i]++;
		
	for(int i = 0;i < numRows;i++)
		aRows[i + 1] += aRows[i];
}

void matrix::print(int* rows, int* cols, double* values, int rank) {
	std::cout << "Process " << rank << ":" << std::endl;
	for(int i = 0;i < numRows + 1;i++)
		std::cout << rows[i] << " ";
		
	std::cout << std::endl;	
}

void matrix::spmv() {
}
