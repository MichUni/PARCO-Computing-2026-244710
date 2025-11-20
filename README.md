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
├── plots/
│   ├── */        # directories containing all the plots 
│   └── *.py      # python scripts for plots creation
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
> Don't know if you have it? Open your `cmd` and run this command:
> ```
> gcc --version
> ```
> if you get `command not found` or a similar error you may need to install it

###### OPTIONAL | only for plots
- **`python3` and its libraries: `pandas`, `seaborn`, `matplotlib`**
> Don't know if you have them? Open your `cmd` and run these commands:
> ```
> python3 --version
> python3 -c "import pandas as pd; print(pd.__version__)"
> python3 -c "import seaborn as sns; print(sns.__version__)"
> python3 -c "import matplotlib; print(matplotlib.__version__)"
> ```
> if you get `command not found` or similar errors you may need to install them before making any plot

## ⚡ Usage

1. clone the repo
```
git clone https://github.com/MichUni/PARCO-Computing-2026-244710/
```


matrices must be added in the **deliverable/matrices** directory, the matrices must me named **matrix<id>.txt**

https://drive.google.com/drive/folders/1nl7Y6W9WeGmjKlK-g7qocmWW5aihjrA1?usp=sharing
