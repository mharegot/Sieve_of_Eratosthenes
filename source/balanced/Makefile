# makefile for the Sieve of Eratosthenes: builds a balanced parallel version
# make 		   # compiles
# make clean       # to clean up built stuff

#-----------------------------------------------------------------------
# General options:
#-----------------------------------------------------------------------
CC      = gcc
# compile with compiler debugging flags:
OPT       = -Wall -g
# compiler with compiler optimzation flags:
#OPT       = -Wall -O2
#C_OMP     = -fopenmp 

#-----------------------------------------------------------------------
# Options specific to C
#-----------------------------------------------------------------------
STD       = -std=gnu9x   # C99 plus GNU extensions (needed for getopt)


CFLAGS  = $(OPT) $(STD)
FFLAGS  = $(OPT) $(FORMAT)
LDFLAGS = $(OPT)
#Libmath
LIBS    = -lm -pthread
BALANCED       = bal_sieve
SRC = balancedSieve.c

.IGNORE:

.SUFFIXES: .f90 .c .o

build: $(BALANCED)

$(BALANCED): $(SRC)
	$(CC) -o $(BALANCED) $(SRC) $(LDFLAGS) $(LIBS)

clean:
	$(RM) *.o $(BALANCED)
