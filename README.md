# GTBarriers-Barrier-Synchronization #

In parallel computing, a barrier is a type of synchronization method. A barrier for a group of threads or processes in the source code means any thread/process must stop at this point and cannot proceed until all other threads/processes reach this barrier. 

The objective of the project is to implement various barrier synchronization algorithms, perform detailed experiments on them and do the analysis. There are various barrier synchronization algorithms each of which has it’s pros and cons. In this project we try to achieve following goals: 
 
1. Implement two barrier synchronization algorithms to synchronize between multiple threads using OpenMP (Open Multi­Processing) framework on a shared memory multiprocessing machine. 

2. Implement two barrier synchronization algorithms to synchronize between multiple processes using MPI (Message passing interface) framework. 

3. Combine two of the barrier synchronization from the above two and synchronize between multiprocessing multithreaded environment. 

4. Experiment all the above with various threads and process sizes, present the results and analyze them learn about OpenMP and MPI frameworks
