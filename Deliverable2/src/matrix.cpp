#include "matrix.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <omp.h>

matrix::matrix(std::string fileName) {
    std::ifstream file(fileName);

    if(!file.is_open()) {
        std::cerr << "Error: could not open file " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }

    while(file.peek() == '%')
        file.ignore(2048, '\n');

    bool isSorted = file.peek() == '!';

    if(isSorted)
        file.ignore(2048, '\n');

    int numValues;
    file >> numRows >> numCols >> numValues;

    aRows = new int[numRows + 1];
    std::fill_n(aRows, numRows + 1, 0);

    aCols = new int[numValues];
    values = new double[numValues];

    if(isSorted) {
        int row, col;
        double val;

        for(int i = 0;i < numValues;i++) {
            file >> row >> col >> val;

            aRows[row]++;
            aCols[i] = col - 1;
            values[i] = val;
        }

        file.close();
    } else {
        struct tuple {
            int row;
            int col;
            double val;
        };

        std::vector<tuple> tuples(numValues);

        for(int i = 0;i < numValues;i++) {
            file >> tuples[i].row >> tuples[i].col >> tuples[i].val;
        }

        file.close();

        std::sort(tuples.begin(), tuples.end(), [](const tuple& a, const tuple& b) {
            if(a.row == b.row)
                return a.col < b.col;

            return a.row < b.row;
        });

        std::ofstream outfile("matrices/temp.mtx");
        outfile << "!sorted\n" << numRows << " " << numCols << " " << numValues << "\n";

        for(int i = 0;i < numValues;i++) {
            tuple t = tuples[i];

            aRows[t.row]++;
            aCols[i] = t.col - 1;
            values[i] = t.val;

            outfile << t.row << " " << t.col << " " << t.val << std::endl;
        }

        outfile.close();
        std::remove(fileName.c_str());
        std::rename("matrices/temp.mtx", fileName.c_str());
    }

    for(int i = 1;i < numRows;i++) {
        aRows[i + 1] += aRows[i];
    }

    productArr = new double[numCols];
    for(int i = 0;i < numCols;i++)
        productArr[i] = rand();
        
    resultArr = new double[numRows];
    std::fill_n(resultArr, numRows, 0);
}

matrix::~matrix() {
    delete[] aRows;
    delete[] aCols;
    delete[] values;

    delete[] productArr;
    delete[] resultArr;
}

void matrix::sequentialProduct() {
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
