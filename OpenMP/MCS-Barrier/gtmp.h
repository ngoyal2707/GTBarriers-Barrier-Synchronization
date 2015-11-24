#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

typedef struct MCSLock {
    int parentsense;
    int* parentpointer;
    int* childpointer[2];
    int havechildren[4];
    int childnotready[4];
    int dummy;
}mylock;

void gtmp_init(int num_threads, mylock lock[]);

void gtmp_barrier(int pid, int num_threads,  mylock lock[], int* sense);
