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

# output CSV file
outputFile="results/perfResults.csv"
echo "matrix,threads,mode,chunkSize,cycles,instructions,cache-references,cache-misses,LLC-loads,LLC-load-misses" > "$outputFile"

execute() {
    	args=("$@")
    	runs=5
    
    	local output
    	output=$(perf stat -r $runs -e cycles,instructions,cache-references,cache-misses,LLC-loads,LLC-load-misses -- ./src/matrixProduct "${args[@]}" 2>&1 >/dev/null)

    	local cycles=$(echo "$output" | grep "cycles" | awk '{print $1}' | sed 's/,//g')
    	local instructions=$(echo "$output" | grep "instructions" | awk '{print $1}' | sed 's/,//g')
    	local cache_refs=$(echo "$output" | grep "cache-references" | awk '{print $1}' | sed 's/,//g')
    	local cache_misses=$(echo "$output" | grep "cache-misses" | awk '{print $1}' | sed 's/,//g')
    	local llc_loads=$(echo "$output" | grep "LLC-loads" | awk '{print $1}' | sed 's/,//g')
    	local llc_misses=$(echo "$output" | grep "LLC-load-misses" | awk '{print $1}' | sed 's/,//g')

    	cycles=${cycles:-0}
    	instructions=${instructions:-0}
    	cache_misses=${cache_misses:-0}
    	cache_refs=${cache_refs:-0}
	llc_loads=${llc_loads:-0}
    	llc_misses=${llc_misses:-0}

    	echo "$cycles,$instructions,$cache_refs,$cache_misses,$llc_loads,$llc_misses"
}

for matrix in "${matricesArr[@]}"; do
	for threads in "${threadsArr[@]}"; do
        	if [ "$threads" -eq 1 ]; then
            		# sequential run (mode and chunkSize irrelevant)
            		averages=$(execute "$matrix" "$threads" 1)
            		echo "$matrix,$threads,-1,-1,$averages" >> "$outputFile"
            		continue
        	fi

        	for mode in "${modeArr[@]}"; do
            		for chunkSize in "${chunkSizeArr[@]}"; do
                		averages=$(execute "$matrix" "$threads" "$mode" "$chunkSize" 1)
                		echo "$matrix,$threads,$mode,$chunkSize,$averages" >> "$outputFile"
            		done
        	done
    	done
done