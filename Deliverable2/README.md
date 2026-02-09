# Sparse Matrix Vector Multiplication

Sparse Matrix Vector Multiplication is a fundamental operation used in a widespread amount of applications.

This project aims to evaluate the performance of both sequential and parallel versions of SpMV under different combinations, using C++ and MPI for parallelization.

## 📑 Matrices

The benchmark is executed on five different matrices, downloaded from the [Suit Sparse Matrix Collection](https://sparse.tamu.edu/) with the `.mtx` format

- [PR02R](https://sparse.tamu.edu/Fluorem/PR02R)
- [TSOPF_RS_b678_c1](https://sparse.tamu.edu/TSOPF/TSOPF_RS_b678_c1)
- [Zd_Jac3](https://sparse.tamu.edu/VanVelzen/Zd_Jac3)
- [16614](https://sparse.tamu.edu/FIDAP/ex11)
- [hcircuit](https://sparse.tamu.edu/Hamm/hcircuit)

## 📂 File Structure
```
Deliverable2
├── matrices/
│   ├── matrix-st*.mtx              # matrices used for strong scaling SpMV
│   └── matrix-wk*.mtx              # matrices used for weak scaling SpMV
├── plots/
│   ├── */                          # directories containing all the plots 
│   └── *.py                        # python scripts for plots creation
├── results/
│   ├── strongScalingResults.csv
│   └── weakScalingResults.csv
├── scripts
│   ├── strongScaling.sh
│   ├── weakScaling.sh
│   ├── strongScaling.pbs
│   └── weakScaling.pbs
├── src/
│   ├── matrixGenerator/            # generate matrices for weak scaling
│   │   └── matrixGenerator.cpp
│   ├── matrixSort/                 # check if matrices are sorted before inizialing SpMV
│   │   ├── matrixSort.cpp
│   │   └── matrixSort.h
│   ├── main.cpp
│   ├── matrix.cpp
│   ├── matrix.h
│   ├── ghostEntries.h
│   ├── ghostEntries.g
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
- `MPI Implementation`
> Don't know if you have it? Open your `terminal` and run this command:
> ```
> mpicxx --version
> ```
> if you get `command not found` or a similar error you may need to install it

###### OPTIONAL | only for plots
- **`python3` and its libraries: `pandas`, `seaborn`, `matplotlib`**
> Don't know if you have them? Open your `terminal` and run these commands:
> ```
> python --version
> python -c "import pandas as pd; print(pd.__version__)"
> python -c "import seaborn as sns; print(sns.__version__)"
> python -c "import matplotlib; print(matplotlib.__version__)"
> ```
> if you get `command not found` or similar errors you may need to install them before making any plot

## ⚡ Usage

1. #### DOWNLOAD
Download the repository directly from [github](https://github.com/MichUni/PARCO-Computing-2026-244710/tree/main) or clone it:
```
git clone https://github.com/MichUni/PARCO-Computing-2026-244710/
```
In you cloned the repository then you have to download the `matrices/` zipped folder manually from [google drive](https://drive.google.com/drive/folders/1KHNlM3dX-ATJnIBEgEguei3XbxLDdoZx?usp=sharing) and unzip it into the cloned folder or you can download them from the `📑 Matrices` section above, but in this case you have to rename them to `matrix_st0.mtx`, `matrix_st1.mtx`, `matrix_st2.mtx`, `matrix_st3.mtx` and `matrix_st4.mtx`.
###### make sure that the `matrices/` folder location corresponds to the project structure saw above

2. #### EXECUTION
The project can be run on two different environments
###### LOCAL
To run the benchmark **locally** open your terminal and execute the `bash` files:
```
chmod -x scripts/strongScaling.sh
chmod -x scripts/weakScaling.sh
bash scripts/strongScaling.sh
bash scripts/weakScaling.sh
```
##### you can run the bash files from any directory but the commands above only apply if you are inside the project main folder
###### HPC CLUSTER
To run the benchmark on a **hpc cluster** submit the jobs from the `Deliverable2/` folder:
```
qsub scripts/strongScaling.pbs
qsub scripts/weakScaling.pbs
```
3. #### RESULTS
The benchmark outputs are saved in the `results/` folder
- `strongScalingResults.csv` and `weakScalingResults.csv` contains the gathered data from the SpMV such as `commutation time`, `computation time`, `execution time`, `gflops`, `nnz values per rank` and `ghost entries per rank` for each matrix combination
4. #### PLOTTING
To plot the results run the python scripts:
```
python3 plots/execution_time.py
python3 plots/gflops.py
python3 plots/load_balanca.py
python3 plots/speedup_efficiency.py
```
The plots are saved in the different folders inside the `plots/` directory

## 👤 Contact

- Michele Zanetti [ 244710 ]
- [michele.zanetti-4@studenti.unitn.it](mailto:michele.zanetti-4@studenti.unitn.it)