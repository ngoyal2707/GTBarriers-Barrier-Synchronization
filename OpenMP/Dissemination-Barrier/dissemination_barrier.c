#include <omp.h>
#include <stdio.h>
#include <math.h>
#define MAX_THREADS 20

typedef struct flags
{
	/* data */
	int myFlags[2][MAX_THREADS];
	int *partnerFlags[2][MAX_THREADS];
}flags;


void dissemination_barrier(int *sense, int * parity, flags * localFlags, int num_threads, int numProcs){
	int i;
	for(i=0;i<numProcs;i++){
		*((*localFlags).partnerFlags[(*parity)][i])=(*sense);
		while( (*localFlags).myFlags[(*parity)][i] != (*sense));
	}

	if((*parity)==1)
		(*sense) ^= 1;
	(*parity) = 1-(*parity);  
}

int main(int argc, char **argv)
{
	int num_threads=8;
	int TOTAL_BARRIER_COUNT = 100;


	flags allNodes[num_threads];
	int i,j,k;

	int numProcs = ceil(log(num_threads)/log(2));

	for(i=0;i<num_threads;i++){
		for(k=0;k<numProcs;k++){
			allNodes[i].myFlags[0][k]=0;
			allNodes[i].myFlags[1][k]=0;
		}
	}

#pragma omp parallel num_threads(num_threads) shared(allNodes)
	{

		int i=0;
		int parity = 0;
		int sense = 1;
		flags *localFlags;
		int temp;

		int threadID =  omp_get_thread_num();
		
		localFlags = &allNodes[threadID];


		#pragma omp critical
			
		for(j=0; j<num_threads; j++){
			for(k=0; k<numProcs; k++){
				temp=ceil(pow(2,k));

				if( j == (threadID+temp)%num_threads ){
					allNodes[threadID].partnerFlags[0][k] =  &allNodes[j].myFlags[0][k];
					allNodes[threadID].partnerFlags[1][k] =  &allNodes[j].myFlags[1][k];
				}
			}
		}

		for(i=0;i<TOTAL_BARRIER_COUNT;i++){
			double t1 = omp_get_wtime(); 
			dissemination_barrier(&sense, &parity, localFlags, num_threads, numProcs);
			double t2 = omp_get_wtime();

			printf("This is thread number %d on %d barrier\n",threadID, i );
		}

	}
	return 0;

}

