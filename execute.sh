#!/bin/bash

# Number of matrices
numOfMatrices=2

# Thread counts (1 = serial | 2+ = parallel)
tarr=(1 2 4 8 16 32 64)

# Output file
of="results.csv"

# Clear output file first
echo -n "" > "$of"

for ((matrixId=0; matrixId<numOfMatrices; matrixId++)); do
	echo "matrix $matrixId" >> "$of"

	for nt in "${tarr[@]}"; do
    		if [ "$nt" -eq 0 ]; then
        		echo -n "sequential:               " >> "$of"
    		elif [ "$nt" -lt 10 ]; then
			echo -n "parallel with $nt threads:  " >> "$of"
		else
        		echo -n "parallel with $nt threads: " >> "$of"
	    	fi


        	results=()  # empty array

        	# Run 10 times and store run times
        	for ((i=0; i<10; i++)); do
            		time=$(./deliverable/matrixProduct "$matrixId" "$nt")
            		results+=("$time")
        	done

        	# Sort results numerically
        	sortedResults=($(printf "%s\n" "${results[@]}" | sort -n))

        	# 90th percentile index for 10 runs → element 9*0.9=9 → index 8
        	p90="${sortedResults[8]}"

        	echo "$p90 s" >> "$of"
	done
done
