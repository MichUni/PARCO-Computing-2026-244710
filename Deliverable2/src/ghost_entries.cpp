#include "matrix.h"

#include <mpi.h>
#include <set>

void identify_ghost_entries(const matrix &A, GhostData* &ghostEntries, int size, int rank) {
	ghostEntries->rank = rank;
	ghostEntries->size = size;
	
	std::set<int> columnsNeeded = new set<int>;
	
	ghostEntries->columnsNeededFrom = new int*[size];
	ghostEntries->columnsNeededCount = new int[size];
	
	std::fill_n(ghostEntries->columnsNeededCount, size, 0);
	
	for(int i = 0;i < A.numValues;i++) {
		int owner = A.aCols[i] % size;
		if(owner != rank) {
			if(columnsNeeded.insert(A.aCols[i]).second) {
				ghostEntries->columnsNeededCount[owner]++;
			}
		}
	}
	
	columnsNeeded.clear();
	
	for(int i = 0;i < size;i++) {
		columnsNeededFrom[i] = new int[columnsNeededCount[i]];
	}
	
	std::fill_n(ghostEntries->columnsNeededCount, size, 0);
	
	for(int i = 0;i < A.numValues;i++) {
		int owner = A.aCols[i] % size;
		if(owner != rank) {
			if(columnsNeeded.insert(A.aCols[i]).second) {
				int index = ghostEntries->columnsNeededCount[owner]++;
				ghostEntries->columnsNeededFrom[owner][index] = A.aCols[i];
			}
		}
	}
}


void exchange_ghost_entries(int numGhostEntries, const int* ghostColumns, GhostData* &ghostEntries, const double* localProductArray, int size, int rank, MPI_Comm comm) {
	ghostEntries->columnsToSendCount = new int[size];
	
	MPI_Alltoallv(ghostEntries->columnsNeededCount, 1, MPI_INT, ghostEntries->columnsToSendCount, 1, MPI_INT, comm)
	
	int *sdispls = new int[size];
    int *rdispls = new int[size];
    
    std::fill_n(sdispls, size, 0);
    std::fill_n(rdispls, size, 0);
    
    for(int i = 1;i < size;i++) {
    	sdispls[i] = sdispls[i - 1] + ghostEntries->columnsNedeedCount[i - 1];
    	rdispls[i] = rdispls[i - 1] + ghostEntries->columnsToSendCount[i - 1];
	}
	
	int totalSCount = sdispls[size - 1] + ghostEntries->columnsNedeedCount[size - 1];
	int totalRCount = rdsipls[size - 1] + ghostEntries->columnsToSendCount[size - 1];
	
	columnToGhostIndex = new int[totalSCount];
	int* rBuffer = new int[totalRCount];
	
	int index = 0;
	
	for(int i = 0;i < size;i++) {
		for(int j = 0;j < ghostEntries->columnsNeededCount[i];j++) {
			columnToGhostIndex[index] = ghostEntries->columnsNeededFrom[i][j];
			index++;
		}
	}
	
	MPI_Alltoallv(columnToGhostIndex, ghostEntries->columnsNeededCount, sdispls, MPI_INT, rBuffer, ghostEntries->columnsToSendCount, rdispls, MPI_INT, comm);
	
	ghost_entries->columnsToSendTo = new double[totalRCount];
	
	for(int i = 0;i < totalRCount;i++) {		
		ghost_entries->columnsToSendTo[i] = localProductArray[rBuffer[index] / size];
	}
	
	ghostVector = new double[totalSCount];
	
	MPI_Alltoallv(ghostVector, ghostEntries->columnsToSendCount, rdispls, MPI_INT, rBuffer, ghostEntries->columnsNeededCount, sdispls, MPI_INT, comm);
	
	
	for(int i = 0;i < numGhostEntries;i++) {
		int reqCol = ghostColumns[i];
		int owner = reqCol % size;
		
		MPI_Send(&reqCol, 1, MPI_INT, owner, 0, MPI_COMM_WORLD);
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	for(int i = 0;i < size;i++) {
		if(i == rank)
			continue;
			
		MPI_Status status;
        int flag = 0;

        do {
            MPI_Iprobe(i, 0, MPI_COMM_WORLD, &flag, &status);
            
            if(flag) {
                int reqCol;
                MPI_Recv(&reqCol, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                double value = localProductArray[reqCol / size];
                MPI_Send(&value, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            }
        } while(flag);
	}
	
	for(int i = 0;i < numGhostEntries;i++) {
        int reqCol = ghostColumns[i];
        int owner = reqCol % size;

        MPI_Recv(&ghostEntries[i], 1, MPI_DOUBLE, owner, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}
