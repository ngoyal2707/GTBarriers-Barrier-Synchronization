#include "gtmp.h"

/*
    From the MCS Paper: A scalable, distributed tree-based barrier with only local spinning.

    type treenode = record
        parentsense : Boolean
	parentpointer : ^Boolean
	childpointers : array [0..1] of ^Boolean
	havechild : array [0..3] of Boolean
	childnotready : array [0..3] of Boolean
	dummy : Boolean //pseudo-data

    shared nodes : array [0..P-1] of treenode
        // nodes[vpid] is allocated in shared memory
        // locally accessible to processor vpid
    processor private vpid : integer // a unique virtual processor index
    processor private sense : Boolean

    // on processor i, sense is initially true
    // in nodes[i]:
    //    havechild[j] = true if 4 * i + j + 1 < P; otherwise false
    //    parentpointer = &nodes[floor((i-1)/4].childnotready[(i-1) mod 4],
    //        or dummy if i = 0
    //    childpointers[0] = &nodes[2*i+1].parentsense, or &dummy if 2*i+1 >= P
    //    childpointers[1] = &nodes[2*i+2].parentsense, or &dummy if 2*i+2 >= P
    //    initially childnotready = havechild and parentsense = false
	
    procedure tree_barrier
        with nodes[vpid] do
	    repeat until childnotready = {false, false, false, false}
	    childnotready := havechild //prepare for next barrier
	    parentpointer^ := false //let parent know I'm ready
	    // if not root, wait until my parent signals wakeup
	    if vpid != 0
	        repeat until parentsense = sense
	    // signal children in wakeup tree
	    childpointers[0]^ := sense
	    childpointers[1]^ := sense
	    sense := not sense
*/

void gtmp_init(int num_threads, mylock lock[]){
	int i = 0;
	for (i = 0; i < num_threads; ++i){
		int j = 0;
		for (j = 1; j < 5; ++j){
			if (4*i + j < num_threads){
				lock[i].havechildren[j-1] = 1;
				lock[i].childnotready[j-1] = 1;
			}
			else{
				lock[i].havechildren[j-1] = 0;
				lock[i].childnotready[j-1] = 0;
			}			
		}
		if (i != 0){
			lock[i].parentpointer = &(lock[((i-1)/4)].childnotready[(i-1)%4]);
		}
		if (2*i+1 < num_threads){
			lock[i].childpointer[0] = &(lock[2*i+1].parentsense);
		}
		if (2*i+2 < num_threads){
			lock[i].childpointer[1] = &(lock[2*i+2].parentsense);
		}
		lock[i].parentsense = 0;	
	}
}

void gtmp_barrier(int pid, int num_threads, mylock lock[], int* sense){
	while (lock[pid].childnotready[0] != 0 || lock[pid].childnotready[1] != 0 || lock[pid].childnotready[2] != 0 || lock[pid].childnotready[3] != 0);
	int i = 0;
	for (i = 0; i < 4; ++i){
		lock[pid].childnotready[i] = lock[pid].havechildren[i];
	}
	if (pid != 0){
		*(lock[pid].parentpointer) = 0;
	}
	if (pid != 0){
		while (lock[pid].parentsense != *sense);
	}
	if (2*pid+1 < num_threads){
		*(lock[pid].childpointer[0]) = *sense;
	}
	if (2*pid+2 < num_threads){
		*(lock[pid].childpointer[1]) = *sense;
	}
	*sense = !(*sense);	
}
