#include "matrix.h"
#include <mpi.h>
#include <set>

void identify_ghost_entries(const matrix &A, GhostData &ghostEntries, int size, int rank, MPI_Comm comm) {
    ghostEntries.rank = rank;
    ghostEntries.size = size;

    std::set<int> columnsNeeded;

    ghostEntries.columnsNeededFrom = new int*[size];
    ghostEntries.columnsNeededCount = new int[size];
    std::fill_n(ghostEntries.columnsNeededCount, size, 0);

    for(int i = 0;i < A.numValues;i++) {
        int owner = A.aCols[i] % size;
        if(owner != rank) {
            if(columnsNeeded.insert(A.aCols[i]).second) {
                ghostEntries.columnsNeededCount[owner]++;
            }
        }
    }

    columnsNeeded.clear();

    for(int i = 0;i < size;i++) {
        ghostEntries.columnsNeededFrom[i] = new int[ghostEntries.columnsNeededCount[i]];
    }

    std::fill_n(ghostEntries.columnsNeededCount, size, 0);

    for(int i = 0;i < A.numValues;i++) {
        int owner = A.aCols[i] % size;
        if(owner != rank) {
            if(columnsNeeded.insert(A.aCols[i]).second) {
                int index = ghostEntries.columnsNeededCount[owner]++;
                ghostEntries.columnsNeededFrom[owner][index] = A.aCols[i];
            }
        }
    }

    ghostEntries.columnsToSendCount = new int[size];

    MPI_Alltoall(ghostEntries.columnsNeededCount, 1, MPI_INT, ghostEntries.columnsToSendCount, 1, MPI_INT, comm);

    int *sdispls = new int[size];
    int *rdispls = new int[size];

    std::fill_n(sdispls, size, 0);
    std::fill_n(rdispls, size, 0);

    for(int i = 1;i < size;i++) {
        sdispls[i] = sdispls[i - 1] + ghostEntries.columnsNeededCount[i - 1];
        rdispls[i] = rdispls[i - 1] + ghostEntries.columnsToSendCount[i - 1];
    }

    ghostEntries.numGhostEntries = sdispls[size - 1] + ghostEntries.columnsNeededCount[size - 1];
    int totalRCount = rdispls[size - 1] + ghostEntries.columnsToSendCount[size - 1];

    ghostEntries.columnToGhostIndex = new int[ghostEntries.numGhostEntries];
    int* rBuffer = new int[totalRCount];

    int index = 0;

    for(int i = 0;i < size;i++) {
        for(int j = 0; j < ghostEntries.columnsNeededCount[i]; j++) {
            ghostEntries.columnToGhostIndex[index] = ghostEntries.columnsNeededFrom[i][j];
            index++;
        }
    }

    MPI_Alltoallv(ghostEntries.columnToGhostIndex, ghostEntries.columnsNeededCount, sdispls, MPI_INT, rBuffer, ghostEntries.columnsToSendCount, rdispls, MPI_INT, comm);

    ghostEntries.columnsToSendTo = new int*[size];

    index = 0;

    for(int i = 0;i < size;i++) {
        ghostEntries.columnsToSendTo[i] = new int[ghostEntries.columnsToSendCount[i]];
        for(int j = 0; j < ghostEntries.columnsToSendCount[i]; j++) {
            ghostEntries.columnsToSendTo[i][j] = rBuffer[index];
            index++;
        }
    }

    delete[] rBuffer;

    for(int i = 0;i < size;i++) {
        delete[] ghostEntries.columnsNeededFrom[i];
    }

    delete[] ghostEntries.columnsNeededFrom;
    delete[] sdispls;
    delete[] rdispls;
}

void exchange_ghost_entries(GhostData &ghostEntries, const double* localProductArray, MPI_Comm comm) {
    int size = ghostEntries.size;

    int *sdispls = new int[size];
    int *rdispls = new int[size];

    std::fill_n(sdispls, size, 0);
    std::fill_n(rdispls, size, 0);

    for(int i = 1;i < size;i++) {
        sdispls[i] = sdispls[i - 1] + ghostEntries.columnsToSendCount[i - 1];
        rdispls[i] = rdispls[i - 1] + ghostEntries.columnsNeededCount[i - 1];
    }

    int totalSCount = sdispls[size - 1] + ghostEntries.columnsToSendCount[size - 1];

    double* sBuffer = new double[totalSCount];

    int index = 0;

    for(int i = 0;i < size;i++) {
        for(int j = 0; j < ghostEntries.columnsToSendCount[i]; j++) {
            sBuffer[index] = localProductArray[ghostEntries.columnsToSendTo[i][j] / size];
            index++;
        }
    }

    ghostEntries.ghostVector = new double[ghostEntries.numGhostEntries];

    MPI_Alltoallv(sBuffer, ghostEntries.columnsToSendCount, sdispls, MPI_DOUBLE, ghostEntries.ghostVector, ghostEntries.columnsNeededCount, rdispls, MPI_DOUBLE, comm);

    delete[] sBuffer;
    delete[] sdispls;
    delete[] rdispls;
}

void free(GhostData &ghostEntries) {
    delete[] ghostEntries.columnsNeededCount;

    for(int i = 0;i < ghostEntries.size;i++) {
        delete[] ghostEntries.columnsToSendTo[i];
    }

    delete[] ghostEntries.columnsToSendCount;
    delete[] ghostEntries.ghostVector;
    delete[] ghostEntries.columnToGhostIndex;
}
