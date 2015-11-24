#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "mpi.h"

void dissemination_lock(int my_id, int num_processor){
	int logP = ceil(log(num_processor)/log(2));
	int i = 0, rank =0;
	int val;
	for (i = 0; i < logP; ++i){
		int sendProc = (my_id + (int)pow(2,i))%num_processor;
		int recvProc = my_id - (int)pow(2,i);
		while (recvProc < 0){
			recvProc += num_processor;
		}
		MPI_Send(&i,1,MPI_INT,sendProc,0,MPI_COMM_WORLD);
		MPI_Recv(&val,1,MPI_INT,recvProc,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		if (val != i){
			printf("Error in Recieved Message\n");
		}
	}
}

int main(int argc, char **argv)
{
  int my_id, num_processes, name_len;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);
	int i = 0;
	double time1, time2;
	double time = 0;
	for (i = 0; i < 1000; ++i){
		MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
		MPI_Get_processor_name(processor_name,&name_len);

		time1 = MPI_Wtime();
		dissemination_lock(my_id, num_processes);
		time2 = MPI_Wtime();
		time = time + (time2 - time1);
		//printf("Hello World from process ranked %d of %d during iteration %d\n", my_id, num_processes, i+1);
	}
	printf("The time spent by processor %d at barrier at is %f\n", my_id, time);
  MPI_Finalize();
  return 0;
}

