#include <iostream>
#include <omp.h>

#include "matrix.h"
#include "timer.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int matrixId;
    int numOfThreads;
    int mode;
    int chunkSize;
    int numOfRuns;

    if(argc == 4) {
        matrixId = atoi(argv[1]);
        numOfThreads = atoi(argv[2]);

        if(numOfThreads != 1) {
            std::cerr << "wrong number of threads" << std::endl;
            return 1;
        }

        numOfRuns = atoi(argv[3]);

        if(numOfRuns < 1) {
            std::cerr << "wrong number of runs" << std::endl;
            return 1;
        }
    } else if(argc == 6) {
        matrixId = atoi(argv[1]);
        numOfThreads = atoi(argv[2]);

        if(numOfThreads <= 1) {
            std::cerr << "wrong number of threads" << std::endl;
            return 1;
        }

        mode = atoi(argv[3]);

        if(mode < 0 || mode > 2) {
            std::cerr << "wrong mode" << std::endl;
            return 1;
        }

        chunkSize = atoi(argv[4]);

        if(chunkSize < 1) {
            std::cerr << "wrong chunk size" << std::endl;
            return 1;
        }

        numOfRuns = atoi(argv[5]);

        if(numOfRuns < 1) {
            std::cerr << "wrong number of runs" << std::endl;
            return 1;
        }

        switch(mode) {
            case 0:
                omp_set_schedule(omp_sched_static, chunkSize);
                break;
            case 1:
                omp_set_schedule(omp_sched_dynamic, chunkSize);
                break;
            case 2:
                omp_set_schedule(omp_sched_guided, chunkSize);
                break;
        }
    } else {
        std::cerr << "wrong number of arguments" << std::endl;
        std::cerr << argc << " | " << argv[1] << " | " << argv[2] << std::endl;
        return 1;
    }

    matrix m("matrices/matrix" + std::to_string(matrixId) + ".mtx");

    for(int i = 0;i < numOfRuns;i++) {
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
        std::cout << time << std::endl;

        delete[] result;
    }
    
    return 0;
}