int identify_ghost_entries(const matrix &A, int* &ghost_entries, int size, int rank) {
	std::set<int> ghost_set;
	
	for(int i = 0;i < A.numValues;i++) {
		if(A.aCols[i] % size != rank) {
			ghost_set.insert(A.aCols[i]);
		}
	}
	
	ghost_entries = new int[ghost_set.size()];
	
	int index = 0;
	for(std::set<int>::iterator it = ghost_set.begin();it != ghost_set.end();it++) {
		ghost_entries[index++] = *it;
	}
	
	return ghost_set.size();
}

void exchange_ghost_entries(int numGhostEntries, const int* ghostColumns, double* &ghostEntries, const double* localProductArray, int size, int rank) {
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
