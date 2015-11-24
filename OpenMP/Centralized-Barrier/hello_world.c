#include "gtmp.h"

#define NUM_THREADS 6

int main(int argc, char **argv)
{
  // Serial code
	int count, sense, localsense;
	double time1, time2;
  printf("This is the serial section\n");
  omp_set_num_threads(NUM_THREADS);

	gtmp_init(NUM_THREADS, &count, &sense);
/*	gtmp_init(NUM_THREADS);*/
#pragma omp parallel shared(count, sense) private(localsense, time1, time2)
  {
	// Now we're in the parallel section
  int num_threads = omp_get_num_threads();
  int thread_num = omp_get_thread_num();
	double time = 0;
	localsense = 1;
	int i = 0;
	for (i = 0; i < 1000; ++i)
	{
		int j = 0;
		for(j = 0; j < 1000000; j++);
    printf("Hello World from thread %d of %d during iteration %d.\n", thread_num, num_threads, i + 1);
		time1 = omp_get_wtime();
		gtmp_barrier(thread_num, num_threads, &count, &sense, &localsense);
		time2 = omp_get_wtime();
		time = time + time2 - time1; 
	}
	printf("Time spent in barrier by thread %d is %f\n", thread_num, time);
  } // implied barrier

  // Resume serial code
  printf("Back in the serial section again\n");
  return 0;
}
