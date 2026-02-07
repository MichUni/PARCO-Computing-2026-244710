#include <iostream>
#include <fstream>
#include <string>
#include <mpi.h>
#include <cstdlib>
#include <ctime>

#include "matrix.h"
#include "timer.h"

#define MAX_RAND 1000
#define MIN_RAND -1000

int main(int argc, char *argv[]) {
  int matrixId;
	
  MPI_Init(&argc, &argv);

  int rank, size;
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
    matrixId = atoi(argv[1]);
  }
  
  int numRows, numCols, numValues;
  int* rows = nullptr;
  int* cols = nullptr;
  double* values = nullptr;

	if(rank == 0) {
		std::string fileName = "matrices/matrix" + std::to_string(matrixId) + ".mtx";
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
    	
    	std::cout << numValues << std::endl;
    	
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
	if(rank == 0)
		A.print(localRows, localCols, localValues, rank);
		
	double* localProductArray = new double(localNumCols);
	double* localResultArray = new double(localNumRows);	
	
  	srand(time(NULL) + rank);
  	
  	for(int i = 0;i < localNumCols) {
  		localProductArray[i] = ((double)(rand() % (MAX_RAND - MIN_RAND + 1) + MIN_RAND) / 100.0;
	}
	
	identify_ghost_entries();
	exchange_ghost_entries();
	
	if(rank == 0) {
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
	
	MPI_Finalize();
    
    return 0;
}
