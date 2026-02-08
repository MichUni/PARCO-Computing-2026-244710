#ifndef GHOST_ENTRIES_H
#define GHOST_ENTRIES_H

#include <set>
#include "matrix.h"

struct GhostData {
    int rank;
    int size;
    
    int numGhostEntries;

    int **columnsNeededFrom;   		// columns we request from other ranks
    int *columnsNeededCount;   		// count per rank

    double **columnsToSendTo;       // columns values we must send to other ranks
    int *columnsToSendCount;		// count per rank

    double *ghostVector;       		// concatenated ghost values
    int *columnToGhostIndex;   		// maps global column to ghostVector index
};

int identify_ghost_entries(const matrix &A, GhostData &ghostEntries, int size, int rank);
void exchange_ghost_entries(int numGhostEntries, GhostData &ghostEntries, const double* localProductArray);
void free(GhostData &ghostEntries);

#endif
