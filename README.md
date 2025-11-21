# Sparse Matrix Vector Multiplication

Sparse Matrix Vector Multiplication is a fundamental operation used in a widespread amount of applications.

This project aims to evaluate the performance of both sequential and parallel versions of SpMV under different combinations, using C++ and OpenMP for parallelization.

## 📑 Matrices

The benchmark is executed on five different matrices, downloaded from the [Suit Sparse Matrix Collection](https://sparse.tamu.edu/) with the `.mtx` format

- [PR02R](https://sparse.tamu.edu/Fluorem/PR02R)
- [TSOPF_RS_b678_c1](https://sparse.tamu.edu/TSOPF/TSOPF_RS_b678_c1)
- [Zd_Jac3](https://sparse.tamu.edu/VanVelzen/Zd_Jac3)
- [16614](https://sparse.tamu.edu/FIDAP/ex11)
- [hcircuit](https://sparse.tamu.edu/Hamm/hcircuit)

## 📂 File Structure
```
.
├── matrices/
│   └── matrix*.mtx   # matrices used for SpVM
├── plots/
│   ├── */            # directories containing all the plots 
│   └── *.py          # python scripts for plots creation
├── results/
│   ├── perfResults.csv
│   └── timingsResults.csv
├── scripts
│   ├── perfBenchmark.pbs
│   └── timingsBenchmark.pbs
├── src/
│   ├── main.cpp
│   ├── matrix.cpp
│   ├── matrix.h
│   ├── matrixProduct
│   └── timer.h
└── README.md
```

## 📜 Requirements
- `GCC compiler`
> Don't know if you have it? Open your `terminal` and run this command:
> ```
> gcc --version
> ```
> if you get `command not found` or a similar error you may need to install it

###### OPTIONAL | only for plots
- **`python3` and its libraries: `pandas`, `seaborn`, `matplotlib`**
> Don't know if you have them? Open your `terminal` and run these commands:
> ```
> python3 --version
> python3 -c "import pandas as pd; print(pd.__version__)"
> python3 -c "import seaborn as sns; print(sns.__version__)"
> python3 -c "import matplotlib; print(matplotlib.__version__)"
> ```
> if you get `command not found` or similar errors you may need to install them before making any plot

## ⚡ Usage

1. #### DOWNLOAD
Download the repository directly from [github](https://github.com/MichUni/PARCO-Computing-2026-244710/tree/main) or clone it:
```
git clone https://github.com/MichUni/PARCO-Computing-2026-244710/
```
In you cloned the repository then you have to download the `matrices/` folder manually from [github](https://github.com/MichUni/PARCO-Computing-2026-244710/tree/main)
or
download the zip file from [google drive](https://drive.google.com/drive/folders/1nl7Y6W9WeGmjKlK-g7qocmWW5aihjrA1?usp=sharing) and unzip it into the cloned folder
###### make sure that the `matrices/` folder location corresponds to the project structure saw above

2. #### EXECUTION
The project can be run on two different environments
###### LOCAL
To run the benchmark **locally** open your terminal and execute the `bash` files:
```
chmod -x scipts/timingsBenchmark.sh
chmod -x scripts/perfBenchmark.sh
bash scripts/timingsBenchmark.sh
bash scripts/perfBenchmark.sh
```
##### you can run the bash files from any directory but the commands above  only apply if you are inside the project main folder
###### HPC CLUSTER
To run the benchmark on a **hpc cluster** submit the jobs:
```
qsub scripts/timingsBenchmark.pbs
qsub scripts/perfBenchmark.pbs
```
3. #### RESULTS
The benchmark outputs are saved in the `results/` folder
- `timingsResults.csv` contains the execution time for each matrix combination\
- `perfResults.csv` contains the number of `cycles`, `instructions`, `cache-references`, `cache-misses`, `LLC-loads` and `LLC-load-misses` for each matrix combination
4. #### PLOTTING
To plot the results run the python scripts:
```
python3 plots/cache_miss_rate.py
python3 plots/execution_time.py
python3 plots/execution_time_heatmap.py
python3 plots/llc_miss_rate.py
```
The plots are saved in the different folders inside the `plots/` directory

## 👤 Contact

- Michele Zanetti [ 244710 ]
- [michele.zanetti-4@studenti.unitn.it](mailto:michele.zanetti-4@studenti.unitn.it)