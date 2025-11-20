# Sparse Matrix Vector Multiplication

Sparse Matrix Vector Multiplication is a fundamental operation used in a widespread amount of applications.
This project aims to evaluate the performance of both sequential and parallel versions of SpMV under different combinations, using C++ and OpenMP for parallelization.

## Matrices

The benchmark is executed on five different matrices

[PR02R](https://sparse.tamu.edu/Fluorem/PR02R)
[TSOPF_RS_b678_c1](https://sparse.tamu.edu/TSOPF/TSOPF_RS_b678_c1)
[Zd_Jac3](https://sparse.tamu.edu/VanVelzen/Zd_Jac3)
[16614](https://sparse.tamu.edu/FIDAP/ex11)
[hcircuit](https://sparse.tamu.edu/Hamm/hcircuit)

matrices must be added in the **deliverable/matrices** directory, the matrices must me named **matrix<id>.txt**

https://drive.google.com/drive/folders/1nl7Y6W9WeGmjKlK-g7qocmWW5aihjrA1?usp=sharing

py -m pip install pandas seaborn matplotlib
