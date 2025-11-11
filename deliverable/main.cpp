#include <iostream>

#include "matrix.h"
#include "timer.h"

int main() {
    srand(time(NULL));

    for(int i = 0;i < 1;i++) {
        std::cout << "Matrix n." << i << std::endl;

        matrix m("matrices/matrix" + std::to_string(i) + ".mtx");

        double start, finish;

        int serialLength;
        double* resultSequential;

        std::cout << "starting sequential product" << std::endl;
        GET_TIME(start);
        m.sequentialProduct(resultSequential, serialLength);
        GET_TIME(finish);

        double sequentialTime = finish - start;

        int parallelLength;
        double* resultParallel;

        std::cout << "starting parallel product" << std::endl;
        GET_TIME(start);
        m.parallelProduct(resultParallel, parallelLength, 2);
        GET_TIME(finish);

        double parallelTime = finish - start;

        std::cout << "sequential: " << sequentialTime << "s" << std::endl;
        std::cout << "parallel: " << parallelTime << "s" << std::endl;

        delete[] resultSequential;
        delete[] resultParallel;
    }
    
    return 0;
}
