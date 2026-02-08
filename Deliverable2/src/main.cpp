#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include <mpi.h>

#include "matrix.h"
#include "ghost_entries.h"
#include "timer.h"

#define ITERATIONS 11

#define MAX_RAND 1000
#define MIN_RAND -1000

int compareDoubles(const void* a, const void* b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    if(da < db) return -1;
    if(da > db) return 1;
    return 0;
}

int main(int argc, char *argv[]) {
	std::string matrixName;
	int rank, size;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(argc != 2) {
		if(rank == 0) {
		  	std::cerr << "wrong number of arguments" << std::endl;
		  	for(int i = 0;i < argc;i++) {
	  			std::cerr << argv[i] << std::endl;
			}
		}
		
		MPI_Finalize();
		return 1;
	} else if(rank == 0) {
		matrixName = argv[1];
	}
	
	int numRows, numCols, numValues;
	int* rows = nullptr;
	int* cols = nullptr;
	double* values = nullptr;

	if(rank == 0) {
		std::string fileName = "matrices/matrix" + matrixName + ".mtx";
		std::ifstream file(fileName);

	    if(!file.is_open()) {
	        std::cerr << "Error: could not open file " << fileName << std::endl;
	        MPI_Abort(MPI_COMM_WORLD, -1);
	    }
	    
	    while(file.peek() == '%')
        	file.ignore(2048, '\n');

    	bool isSorted = file.peek() == '!';

    	if(isSorted)
        	file.ignore(2048, '\n');
        	
    	file >> numRows >> numCols >> numValues;
    	
    	rows = new int[numValues];
    	cols = new int[numValues];
    	values = new double[numValues];
    	
    	for(int i = 0;i < numValues;i++) {
            file >> rows[i] >> cols[i] >> values[i];
            rows[i]--;
            cols[i]--;
		}
            
        file.close();
	}
	
	MPI_Bcast(&numRows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&numCols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&numValues, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int* numValuesProcess = new int[size];
    std::fill_n(numValuesProcess, size, 0);
    
    if(rank == 0)
    	for(int i = 0;i < numValues;i++)
    		numValuesProcess[rows[i] % size]++;
    		
    int localNumValues = 0;
    MPI_Scatter(numValuesProcess, 1, MPI_INT, &localNumValues, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
	int* localRows = new int[localNumValues];
	int* localCols = new int[localNumValues];
	double* localValues = new double[localNumValues];
	
	if(rank == 0) {
		int* tempLocalIndex = new int[size - 1];
    	std::fill_n(tempLocalIndex, size - 1, 0);
    
    	int** tempLocalRows = new int*[size - 1];
    	int** tempLocalCols = new int*[size - 1];
    	double** tempLocalValues = new double*[size - 1];
    	
    	for(int i = 0;i < size - 1;i++) {
    		int tempNumValues = numValuesProcess[i + 1];
    		tempLocalRows[i] = new int[tempNumValues];
    		tempLocalCols[i] = new int[tempNumValues];
    		tempLocalValues[i] = new double[tempNumValues];
		}
		
		int myIndex = 0;
    	
		for(int i = 0;i < numValues;i++) {
			int owner = rows[i] % size;
			
			if(owner == 0) {
				localRows[myIndex] = rows[i] / size;
				
				localCols[myIndex] = cols[i];
				localValues[myIndex] = values[i];
				myIndex++;
				
				continue;
			}
			
			int tempIndex = tempLocalIndex[owner - 1]++;
			tempLocalRows[owner - 1][tempIndex] = rows[i] / size;
			tempLocalCols[owner - 1][tempIndex] = cols[i];
			tempLocalValues[owner - 1][tempIndex] = values[i];
		}
		
		for(int i = 0;i < size - 1;i++) {
			MPI_Send(tempLocalRows[i], numValuesProcess[i + 1], MPI_INT, i + 1, 0, MPI_COMM_WORLD);
			MPI_Send(tempLocalCols[i], numValuesProcess[i + 1], MPI_INT, i + 1, 1, MPI_COMM_WORLD);
			MPI_Send(tempLocalValues[i], numValuesProcess[i + 1], MPI_DOUBLE, i + 1, 2, MPI_COMM_WORLD);
		}
		
		delete[] tempLocalIndex;
		
		for(int i = 0; i < size - 1; i++) {
		    delete[] tempLocalRows[i];
		    delete[] tempLocalCols[i];
		    delete[] tempLocalValues[i];
		}
		
		delete[] tempLocalRows;
		delete[] tempLocalCols;
		delete[] tempLocalValues;
	} else {
		MPI_Recv(localRows, localNumValues, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    MPI_Recv(localCols, localNumValues, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    MPI_Recv(localValues, localNumValues, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	
	int localNumRows = numRows / size + (rank < (numRows % size) ? 1 : 0);
	int localNumCols = numCols / size + (rank < (numCols % size) ? 1 : 0);
	
	matrix A(localNumRows, localNumValues);
	A.coo_to_csr(localRows, localCols, localValues);
		
	double* localProductArray = new double[localNumCols];
	double* localResultArray = new double[localNumRows];
	
  	srand(time(NULL) + rank);
  	
  	for(int i = 0;i < localNumCols;i++) {
  		localProductArray[i] = ((double)(rand() % (MAX_RAND - MIN_RAND + 1) + MIN_RAND)) / 100.0;
	}
	
	std::fill_n(localResultArray, localNumRows, 0);	
	
	GhostData gd = {0};
	identify_ghost_entries(A, gd, size, rank, numCols, MPI_COMM_WORLD);
	int numGhostEntries = gd.numGhostEntries;
	
	exchange_ghost_entries(gd, localProductArray, MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	for(int i = 0;i < 3;i++) {
		std::fill_n(localResultArray, localNumRows, 0);	
		exchange_ghost_entries(gd, localProductArray, MPI_COMM_WORLD);
		A.spvm(localProductArray, numGhostEntries, gd.columnToGhostIndex, gd.ghostVector, size, rank, localResultArray);
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	double* commutationTime = new double[ITERATIONS];
	double* computationTime = new double[ITERATIONS];
	
	double start, end;
	
	for(int i = 0;i < ITERATIONS;i++) {
		std::fill_n(localResultArray, localNumRows, 0);	
		
		GET_TIME(start);
		exchange_ghost_entries(gd, localProductArray, MPI_COMM_WORLD);
		GET_TIME(end);
		
		commutationTime[i] = (end - start) * 1000.0;
		
		GET_TIME(start);
		A.spvm(localProductArray, numGhostEntries, gd.columnToGhostIndex, gd.ghostVector, size, rank, localResultArray);
		GET_TIME(end);
		
		computationTime[i] = (end - start) * 1000.0;
	}
	
	qsort(commutationTime, ITERATIONS, sizeof(double), compareDoubles);
	qsort(computationTime, ITERATIONS, sizeof(double), compareDoubles);

	int idx90 = (int)(ITERATIONS * 0.9);
	double p90_commutation = commutationTime[idx90];
	double p90_computation = computationTime[idx90];
	
    double max_commutation = 0.0;
    double max_computation = 0.0;
    
    MPI_Reduce(&p90_commutation, &max_commutation, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&p90_computation, &max_computation, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    int min_nz, max_nz;
    
    MPI_Reduce(&localNumValues, &min_nz, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localNumValues, &max_nz, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    
    int min_ge, max_ge;
    int sum_ge;
    MPI_Reduce(&numGhostEntries, &min_ge, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&numGhostEntries, &max_ge, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&numGhostEntries, &sum_ge, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); 
	
	if(rank == 0) {
		double gflops = (2.0 * numValues) / (p90_computation / 1000.0);
		gflops /= 1e9;
    	double avg_nz = ((double)numValues) / size;
    	double avg_ge = ((double)sum_ge) / size;		//avg ghost entries
    	
    	double total_time = max_commutation + max_computation;
    	
    	std::cout << matrixName << "," << size << "," << max_commutation << "," << max_computation << "," << total_time << "," << gflops << "," << min_nz << "," << max_nz << "," << avg_nz << "," << min_ge << "," << max_ge << "," << avg_ge << std::endl;
    	
		delete[] rows;
		delete[] cols;
		delete[] values;
	}
	
	delete[] numValuesProcess;
	
	delete[] localRows;
	delete[] localCols;
	delete[] localValues;
	
	delete[] localProductArray;
	delete[] localResultArray;
	
	delete[] commutationTime;
	delete[] computationTime;
  
  	free_data(gd);
	
	MPI_Finalize();
    
    return 0;
}
