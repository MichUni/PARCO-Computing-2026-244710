#!/bin/bash

cd "$(dirname "$0")/.."

g++ -std=c++11 ./src/main.cpp ./src/matrix.cpp -fopenmp -o ./src/matrixProduct 

# matrices array
matricesArr=(0 1 2 3 4)

# threads array
threadsArr=(1 2 4 8 16 32 64)

# parallel mode array [ static dynamic guided ]
modeArr=(0 1 2)

# chunk size array
chunkSizeArr=(1 10 100 1000)

# open output file and clear it
outputFile="results/timingsResults.csv"
echo "matrix,threads,mode,chunkSize,time" > "$outputFile"

# c++ function
execute() {
	mapfile -t results < <(./src/matrixProduct "$@")
	sortedResults=($(printf "%s\n" "${results[@]}" | sort -n))

	output="${sortedResults[8]}"
	output=$(echo -n "$output" | tr -d '\r\n')
	echo "$output"
}

for matrix in "${matricesArr[@]}"; do
	for threads in "${threadsArr[@]}"; do
		if [ "$threads" -eq 1 ]; then
			time=$(execute "$matrix" "$threads" 11)
			echo "$matrix,$threads,-1,-1,$time" >> "$outputFile"
			continue
		fi
 
		for mode in "${modeArr[@]}"; do
			for chunkSize in "${chunkSizeArr[@]}"; do
				time=$(execute "$matrix" "$threads" "$mode" "$chunkSize" 11)
				echo "$matrix,$threads,$mode,$chunkSize,$time" >> "$outputFile"
			done
		done
	done
done