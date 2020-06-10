## Widening The Sieve
#### Generation of Prime Numbers using the Sieve of Eratosthenes

A prime number is a natural number greater than one that has no other factors 
rather than one and itself. Prime numbers have a variety of applications in 
multiple fields ranging from hashing and public-key cryptography, to mathematical 
research and quantum computing. Large primes, in particular, are crucial for 
security in cryptography, and the generation of such primes is a hot topic as 
a result. Since the distribution of prime numbers over a given interval is to 
this day still unknown, there exist a range of algorithms that allow us to 
generate 
these primes efficiently. In this project, we focus on prime number sieves, 
specifically the Sieve of Eratosthenes, which provides a simple and relatively 
fast method of generating primes from zero up to a maximum positive integer 
*n*. Although this sieve is not embarrassingly parallel, we investigate 
different parallelization approaches using OpenMP, balanced pthreads, 
and CUDA across different interval sizes. We then compare the results from 
these implementations to determine their efficiency in terms of speedup. 
Our results demonstrate that although only part of our algorithm can be 
parallelized, there is a much to gain in efficiency from applying carefully 
constructed parallelism to generate these primes.

The following subdirectories contain the source code, test scripts, results,
and related documents, such as our proposal, midway report and presentation,
and final report and presentation.

> Each source folder has a Makefile within, which can be used to compile and run
each implementation. Note that NVIDIA GPUs are required for our CUDA implementation.

Enjoy!

ProjectProposal/  
MidwayReport/  
FinalReport/  
source/  
tests/  
results/  
