#include "matrixSort.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cerr << "wrong number of parameters" << std::endl;
        return 1;
	}
	
	std::string matrixId = argv[1];
	
	matrixSort m("matrices/matrix" + matrixId + ".mtx");
}

matrixSort::matrixSort(std::string fileName) {
    std::ifstream file(fileName);

    if(!file.is_open()) {
        std::cerr << "Error: could not open file " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }

    while(file.peek() == '%')
        file.ignore(2048, '\n');

    bool isSorted = file.peek() == '!';

    if(isSorted)
        file.ignore(2048, '\n');

    int numValues;
    file >> numRows >> numCols >> numValues;

    if(isSorted) {
        file.close();
        
        return 0;
    } else {
        struct tuple {
            int row;
            int col;
            double val;
        };

        std::vector<tuple> tuples(numValues);

        for(int i = 0;i < numValues;i++) {
            file >> tuples[i].row >> tuples[i].col >> tuples[i].val;
        }

        file.close();

        std::sort(tuples.begin(), tuples.end(), [](const tuple& a, const tuple& b) {
            if(a.row == b.row)
                return a.col < b.col;

            return a.row < b.row;
        });

        std::ofstream outfile("matrices/temp.mtx");
        outfile << "!sorted\n" << numRows << " " << numCols << " " << numValues << "\n";

        for(int i = 0;i < numValues;i++) {
            tuple t = tuples[i];
            outfile << t.row << " " << t.col << " " << t.val << std::endl;
        }

        outfile.close();
        std::remove(fileName.c_str());
        std::rename("matrices/temp.mtx", fileName.c_str());
    }
}
