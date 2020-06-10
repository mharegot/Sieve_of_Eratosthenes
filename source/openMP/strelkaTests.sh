#!/bin/bash

echo " "
echo "Timing Tests For parallel and sequantial runs of OpenMP Sieve"
echo "============================================================="

for n in {1..18}
  do 
    ((SIZE=10**n))
    echo " " 
    echo "n = 1x10^$n " 
    echo "Sequential runtime: " 
    time ./seq_sieve $SIZE
    echo "Parallel runtime: " 
    time ./par_sieve $SIZE
    echo "____________________________________________________________ " 
  done

