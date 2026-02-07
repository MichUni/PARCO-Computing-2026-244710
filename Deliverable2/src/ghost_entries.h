#ifndef GHOST_ENTRIES_H
#define GHOST_ENTRIES_H

#include <vector>
#include "matrix.h"

int identify_ghost_entries(const matrix &A, int* &ghost_entries, int size, int rank);
void exchange_ghost_entries(int numGhostEntries, const int* ghostColumns, double* &ghostEntries, const double* localProductArray, int size, int rank);

#endif
