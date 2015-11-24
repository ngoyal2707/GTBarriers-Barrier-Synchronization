#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void gtmp_init(int num_threads, int* count, int* sense);

void gtmp_barrier(int pid, int num_threads, int* count, int* sense, int* localsense);


