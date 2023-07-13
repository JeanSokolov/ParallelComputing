# Parallel Computing
## C++
### Standard C++ implementation
Source code at OpenMP/src/m2m_serial.cpp
Binary at x86_64-linux-gnu/m2m_serial.o
## OpenMP
### Parallelization on standard C++ implementation
Source code at OpenMP/src/m2m_OpenMP.cpp
Binary at x86_64-linux-gnu/m2m_openmp.o

## MPI
### Variant 1 - static process count
Source code at MPI/src/m2m_static.cpp
Binary at x86_64-linux-gnu/m2m_static_MPI
### Variant 2 - dynamic process count
Source code at MPI/src/Dynamic_proc_count.cpp
Binary at x86_64-linux-gnu/m2m_dynamic_MPI
### Variant 3 - dynamic memory access - memory access at calculation
Receives row of A matrix and column of B matrix at the time of calculation leading to less memory being allocated at runtime.
Source code at MPI/src/B_row_by_row.cpp
Binary at x86_64-linux-gnu/m2m_mem_eff_MPI
## Runtime example
![grafik](https://github.com/JeanSokolov/ParallelComputing/assets/107756820/d054d27d-139a-447d-ae6b-0ecb5ec6c7c3)
