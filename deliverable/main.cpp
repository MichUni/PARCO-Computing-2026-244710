#include <iostream>

#include "matrix.h"
#include "timer.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if(argc != 3) {
        std::cerr << "wrong number of arguments: " << "int1(matrix id), int2(0 = serial, 1+ num of threads)" << std::endl;
        return 1;
    }

    int matrixId = atoi(argv[1]);
    int numOfThreads = atoi(argv[2]);

    matrix m("matrices/matrix" + std::to_string(matrixId) + ".mtx");

    double start, finish;

    int length;
    double* result;

    GET_TIME(start);
    if(numOfThreads == 1)
        m.sequentialProduct(result, length);
    else
        m.parallelProduct(result, length, numOfThreads);
    GET_TIME(finish);

    double time = finish - start;
    std::cout << time;

    delete[] result;
    
    return 0;
}
