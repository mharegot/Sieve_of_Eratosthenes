## Project Work Week (4/12 - 4/18)

Here is a summary of our work and achievements during project work week:
* We parallelized our sequential version of the Sieve of Eratosthenes using OpenMP loop 
  primitives, distributing the workload for marking off multiples of each prime across 
  available threads.
* We ran some preliminary tests to determine correctness of the parallel algorithm, as 
  well as potential speedup of the parallelized code.
* Using information from our initial tests and debugging logs, we noted a job scheduling 
  issue among threads that was causing most of the heavy work to be assigned to a subset
  of threads. We refactored our code to enable dynamic scheduling, effectively distributing 
  the work among the available threads and preventing bottlenecks due to load imbalance.
* We ran a few more tests to verify that our parallel and sequential programs work correctly, 
  and requested for login credentials for the Strelka cluster through ITS. We intend to 
  schedule some Slurm jobs for further tests this weekend once we have the credentials, 
  or early next week.
* We began setting up and working on our CUDA implementation of the parallel sieve, which 
  we will continue this coming week.
* We also began working on a pthreads implementation of the parallel sieve that uses
  a different parallelization technique in order to reduce load imbalance.
