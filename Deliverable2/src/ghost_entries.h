#ifndef GHOST_ENTRIES_H
#define GHOST_ENTRIES_H

#include "matrix.h"

#include <set>
#include <mpi.h>

struct GhostData {
    int rank;
    int size;
    
    int numGhostEntries;

    int **columnsNeededFrom;   		// columns we request from other ranks
    int *columnsNeededCount;   		// count per rank

    int **columnsToSendTo;       // columns values we must send to other ranks
    int *columnsToSendCount;		// count per rank

    double *ghostVector;       		// concatenated ghost values
    int *columnToGhostIndex;   		// maps global column to ghostVector index
};

void identify_ghost_entries(const matrix &A, GhostData &ghostEntries, int size, int rank, int nCols, MPI_Comm comm);
void exchange_ghost_entries(GhostData &ghostEntries, const double* localProductArray, MPI_Comm comm);
void free_data(GhostData &ghostEntries);

#endif
