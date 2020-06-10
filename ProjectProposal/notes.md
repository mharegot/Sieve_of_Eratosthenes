Massey article:
- Block allocation is important, since there is potential for
  a large work imbalance between blocks.
- Complexity is O(n ln ln n), includes runtime analysis
- Contains pseudocode as well as Sieve of Atkin
- Sieve of Eratosthenes created more than 2000 years ago and
  still effective today
- Includes C macros to help implement Block Decomposition method for
  breaking up work
- Optimizations

NASA article:
- from 1982!
- "Because of the nature of the algorithm, it is impossible to get any speedup
  beyond 4 or 5 procesors unless some form of dynamic load balancing is 
  employed"
- Since the actual computation is so simple, any overhead is very prominent
- Often used as a computing benchmark test
- If we wanted to avoid redundant work, the worker assigned to cross out
  multiples of 6 should not go until the workers assigned to 2 and 3 have
  already passed 6.
- Discusses load balancing strategy where idle workers take on half of the
  remaining work of the most overloaded remaining worker.

ResearchGate Article (IEEE):
- Uses OpenCL on top of GPGPU cores to implement parallel Sieve
- Has one process that finds the base primes and broadcasts it
  to other processes so that they can strike out the multiples
- Parallel is 4 times faster than sequential

Notes from Tia:
- Since we are focusing on one algorithm, maybe try test different
  implementations on different systems. Even if it is not something
  we are able to get to, should put this in our proposal.
- It's okay to put things in our proposal that we don't think we will
  ever get to.
- Write about sequential and parallel algorithm in our proposal.
