#!/bin/bash

cd "$(dirname "$0")/.."

g++ -std=c++11 ./src/matrixSort/matrixSort.cpp -o ./src/matrixSort/matrixSort

mpicxx -O3 -Wall ./src/main.cpp ./src/matrix.cpp ./src/ghost_entries.cpp -o ./src/matrixProduct || { echo "Compilation failed"; exit 1; }
chmod +x ./src/matrixProduct

# threads array
threadsArr=(1 2 4 8 16 32 64 128)

# output CSV file
outputFile="results/weakScalingResults.csv"
echo "matrix,processes,max_communication(ms),max_computation(ms),total_time(ms),gflops,min_non_zero_values,max_non_zero_values,avg_non_zero_values,min_ghost_entries,max_ghost_entries,avg_ghost_entries" > "$outputFile"

execute() {
  local output
  output=$(mpirun -np $1 ./src/matrixProduct _wk$1)
  
  echo "$output"
}

for threads in "${threadsArr[@]}"; do
	./src/matrixGenerator/matrixGenerator "$threads"

	result=$(execute "$threads")
    	echo "$result" >> "$outputFile"
done