#include "gtmp.h"

#define NUM_THREADS 6

int main(int argc, char **argv)
{
  // Serial code
	int sense;
	double time1, time2;
	mylock lock[NUM_THREADS];
  printf("This is the serial section\n");
  omp_set_num_threads(NUM_THREADS);

	gtmp_init(NUM_THREADS, lock);
#pragma omp parallel shared(lock) private(sense, time1, time2)
  {
	// Now we're in the parallel section
	sense = 1;
	int num_threads = omp_get_num_threads();
  int thread_num = omp_get_thread_num();
	double time = 0;
	int i = 0;
	for (i = 0; i < 1000; ++i)
	{
		int j = 0;
		for(j = 0; j < 1000; j++);
    		printf("Hello World from thread %d of %d during iteration %d.\n", thread_num, num_threads, i + 1);
		time1 = omp_get_wtime();
		gtmp_barrier(thread_num, num_threads, lock, &sense);
 		time2 = omp_get_wtime();
		time = time + time2 - time1;
	} 
	printf("Time spent in barrier by thread %d is %f\n", thread_num, time);
	} // implied barrier

  // Resume serial code
  printf("Back in the serial section again\n");
  return 0;
}
