#include <stdio.h>
#include <math.h>
#include "mpi.h"
typedef enum {WINNER, LOSER, CHAMPION, BYE} ROLES;
typedef struct round_struct{
  ROLES role;
  int opponent;
}round_struct;

void centralized_barrier_threads(int *sense, int *local_sense, int *count, int num_threads){
  (*local_sense) ^= 1;

// #pragma omp critical
//   (*count) = (*count)-1; 

  if(__sync_fetch_and_add(count,-1) == 1){
    (*count) = num_threads;
    *sense = *local_sense; 
  }else{
    while(*sense != *local_sense);
  }
}


void tournament_barrier_processes(round_struct *rounds, int roundCounts,int my_id, int num_processes, int barrierCount){
  int my_msg[1];
  MPI_Status mpi_result;

  //Arrival phase
  int round_num = 1;
  while(1)
  {
    if(rounds[round_num].role== WINNER|| rounds[round_num].role== CHAMPION)
    {
      MPI_Recv(&my_msg, 1, MPI_INT, rounds[round_num].opponent, barrierCount, MPI_COMM_WORLD, &mpi_result);
    }

    else if(rounds[round_num].role== LOSER)
    {
      MPI_Send(my_msg,1,MPI_INT,rounds[round_num].opponent,barrierCount,MPI_COMM_WORLD);
      MPI_Recv(my_msg,1,MPI_INT,rounds[round_num].opponent,barrierCount,MPI_COMM_WORLD,&mpi_result);
      break;
    }

    if(round_num==roundCounts)
      break;
    else
      round_num++;
  }

  //wakeup tree
  while(1)
  {
    if(round_num==0)
      break;
    if(rounds[round_num].role== WINNER||rounds[round_num].role==CHAMPION)
    MPI_Send(my_msg,1,MPI_INT,rounds[round_num].opponent,barrierCount,MPI_COMM_WORLD);
    round_num--;
  }
}

int main(int argc, char **argv)
{
  int num_threads=4;
  int TOTAL_BARRIER_COUNT = 100;
  int sense = 1, local_sense = 1;
  int count = num_threads;

  int i,j;
  int my_id, num_processes;


  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  int roundCounts = ceil(log(num_processes)/log(2));

  round_struct rounds[roundCounts+1];
  int temp;
  int roundNum;
  double total_time = 0; 

  for(j=0;j<roundCounts;j++){
    roundNum = j+1;
    temp = pow(2, roundNum);
    if(my_id==0 && temp>= num_processes){
      rounds[roundNum].role = CHAMPION;
      rounds[roundNum].opponent = my_id + pow(2, roundNum-1);
    }else if(my_id % temp == 0 &&  (my_id+ pow(2,roundNum-1) < num_processes) && temp < num_processes){
      rounds[roundNum].role = WINNER;
      rounds[roundNum].opponent = my_id + pow(2, roundNum-1);
    }else if(my_id % temp == 0 &&  (my_id+ pow(2,roundNum-1) >= num_processes)){
      rounds[roundNum].role = BYE;
    }else if(my_id % temp == pow(2,roundNum-1)){
      rounds[roundNum].role = LOSER;
      rounds[roundNum].opponent = my_id - pow(2, roundNum-1);
    }
  }


#pragma omp parallel num_threads(num_threads) shared(sense,count) firstprivate(i,local_sense)
{  
  int threadID =  omp_get_thread_num();

  for(i=0;i<TOTAL_BARRIER_COUNT;i++){
      double t1 = MPI_Wtime(); 

#pragma omp master 
{
      tournament_barrier_processes(rounds, roundCounts, my_id, num_processes, i);
}
      centralized_barrier_threads(&sense, &local_sense, &count, num_threads);

      double t2 = MPI_Wtime();

      total_time += (t2-t1);
      printf("This is tid %d of pid %d on %d barrier at time %f\n",threadID, my_id, i,t2 );
  }
}
  printf("Average time taken by processor %d: %f\n",my_id, (total_time/TOTAL_BARRIER_COUNT));

  MPI_Finalize();
  
  return 0;
}

