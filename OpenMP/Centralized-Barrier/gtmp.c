#include "gtmp.h"

/*
    From the MCS Paper: A sense-reversing centralized barrier

    shared count : integer := P
    shared sense : Boolean := true
    processor private local_sense : Boolean := true

    procedure central_barrier
        local_sense := not local_sense // each processor toggles its own sense
	if fetch_and_decrement (&count) = 1
	    count := P
	    sense := local_sense // last processor toggles global sense
        else
           repeat until sense = local_sense
*/

void gtmp_init(int num_threads, int* count, int* sense){
    *count = num_threads;
		*sense = 1;
}

void gtmp_barrier(int pid, int num_threads, int* count, int* sense, int* localsense){
    *localsense = !(*localsense);
    if ( __sync_fetch_and_add(count, -1) == 1){
        *sense = *localsense;
				*count = num_threads;
    }
    else{
        while (*sense != *localsense);
    }
}

