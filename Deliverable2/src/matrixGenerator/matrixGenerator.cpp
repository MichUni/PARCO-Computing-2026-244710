#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#define ROWS_PER_PROCESS 256
#define NNZ_PER_ROW 13

#define MAX_RAND 10000
#define MIN_RAND -10000

int main(int argc, char* argv[]) {        
    if(argc != 2) {
        std::cerr << "wrong number of parameters" << std::endl;
        return 1;
	}

    int p = atoi(argv[1]);
    
    if (p <= 0) {
        std::cerr << "invalid number of processes" << std::endl;
        return 1;
    }

    std::string filename = "matrices/matrix_wk" + std::to_string(p) + ".mtx";
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
    	bool takenSpot[cols];
    	
    	for(int j = 0;j < cols;j++)
    		takenSpot[j] = false;
    	
    	for(int j = 0;j < NNZ_PER_ROW;j++) {
    		int col = rand() % cols;
    		
    		if(!takenSpot[col]) {
    			takenSpot[col] = true;
    			
    			double val = ((double)(rand() % (MAX_RAND - MIN_RAND + 1))) / 1000.0;
    			
    			outfile << (i + 1) << " " << (col + 1) << " " << val << "\n";
			} else {
				j--;
			}
		}
    }

    outfile.close();
    
    return 0;
}

