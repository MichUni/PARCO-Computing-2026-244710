#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#define ROWS_PER_PROCESS 256
#define NNZ_PER_ROW 13

int main(int argc, char* argv[]) {
	std::ofstream outfile("matrices/temp.mtx");
        outfile << "!sorted\n" << numRows << " " << numCols << " " << numValues << "\n";

        for(int i = 0;i < numValues;i++) {
            tuple t = tuples[i];
            outfile << t.row << " " << t.col << " " << t.val << std::endl;
        }

        outfile.close();
        std::remove(fileName.c_str());
        std::rename("matrices/temp.mtx", fileName.c_str());
        
        
    if(argc != 2) {
        std::cerr << "wrong number of parameters" << std::endl;
        return 1;
	}

    int p = atoi(argv[1]);
    
    if (p <= 0) {
        std::cerr << "invalid number of processes" << std::endl;
        return 1;
    }

    std::string filename = "matrices/wk-" + std::to_string(p) + ".mtx";
    std::ofstream outfile(filename);
    
    if (!outfile) {
        std::cerr << "cannot open output file" << std::endl;
        return 1;
    }

    srand(time(NULL));

    int rows = ROWS_PER_PROCESS * p;
    int cols = rows;
    int nnz  = rows * NNZ_PER_ROW;
    
    outfile << "!sorted\n";
    outfile << rows << " " << cols << " " << nnz << "\n";

    for(int i = 0;i < rows;i++) {
    	bool takenSpot[cols] = { 0 };
    	
    	for(int j = 0;j < NNZ_PER_ROW;j++) {
    		int col = rand() % cols;
    		
    		if(!takenSpot[col]) {
    			takenSpot[col] = true;
    			
    			double val = rand() % (MAX_RAND - MIN_RAND + 1)
			}
		}
    }

    out.close();
    return 0;
}

