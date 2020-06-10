# makefile for the Sieve of Eratosthenes: builds a sequential and openMP version
# make 		   # compiles both the parallel and sequential sieve
# make clean       # to clean up built stuff

#-----------------------------------------------------------------------
# General options:
#-----------------------------------------------------------------------
CC      = gcc
# compile with compiler debugging flags:
OPT       = -Wall -g
# compiler with compiler optimzation flags:
#OPT       = -Wall -O2
C_OMP     = -fopenmp 

#-----------------------------------------------------------------------
# Options specific to C
#-----------------------------------------------------------------------
STD       = -std=gnu9x   # C99 plus GNU extensions (needed for getopt)


CFLAGS  = $(OPT) $(STD)
FFLAGS  = $(OPT) $(FORMAT)
LDFLAGS = $(OPT)
#Libmath
LIBS    = -lm 
SEQVERSION       = seq_sieve
PARALLELVERSION  = par_sieve
SRC = sieve.c

.IGNORE:

.SUFFIXES: .f90 .c .o

build: $(PARALLELVERSION) $(SEQVERSION)

run: run-$(PARALLELVERSION)  

$(PARALLELVERSION): $(SRC)
	$(CC) -o $(PARALLELVERSION) $(SRC) $(LDFLAGS)  $(C_OMP) $(LIBS)

$(SEQVERSION): $(SRC)
	$(CC) -o $(SEQVERSION) $(SRC) $(LDFLAGS) $(LIBS)

clean:
	$(RM) *.o $(PARALLELVERSION) $(SEQVERSION)
