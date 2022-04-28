#!/bin/bash
exec gcc bucket_sort.c -lm -o Sequential &
exec gcc bucket_ompd.c -lm -o OpenMP_Dynamic -fopenmp &
exec gcc bucket_omps.c -lm -o OpenMP_Static -fopenmp &
exec mpicc bucket_mpi.c -lm -o MPI &


# {time ./Sequential} |& python3 save_time.py
# {time ./OpenMP_Dynamic} |& python3 save_time.py
# {time ./OpenMP_Static} |& python3 save_time.py
# {time mpirun -np 2 MPI} |& python3 save_time_mpi.py