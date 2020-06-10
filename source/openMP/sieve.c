// TODO: add top-level comment

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

//#define DEBUG
//#define SHOW_RESULTS

#ifdef DEBUG   
#define PRINT0(s) printf(s); fflush(stdout)
#define PRINT1(s, a1) printf(s, a1); fflush(stdout)
#define PRINT2(s, a1, a2) printf(s, a1, a2); fflush(stdout)
#define PRINT3(s, a1, a2, a3) printf(s, a1, a2, a3); fflush(stdout)
#else
#define PRINT0(s) 
#define PRINT1(s, a1) 
#define PRINT2(s, a1, a2) 
#define PRINT3(s, a1, a2, a3) 
#endif

void usage();
void Sieve(char* arr, unsigned long long max);
void printResult(char* arr, unsigned long long max);

int main(int argc, char** argv){
    unsigned long long max;
    char* array;
    if (argc != 2){
        usage();
    }
    max = atoll(argv[1]);
    array = malloc(sizeof(char) * max);
    if(array == NULL){
        printf("malloc failed\n");
        exit(1);
    }
    memset(array, 1, max);
    Sieve(array, max);
#ifdef SHOW_RESULTS 
    printResult(array, max);
#endif
    free(array);
}

void usage(){
    printf("usage: ./sequentialSieve [maxInt]\n");
    exit(1);
}

void Sieve(char* arr, unsigned long long max){
    unsigned long long base;
    unsigned long long next;
    unsigned long long sqrtMax;
    sqrtMax = sqrt(max);
    arr[0] = 0;
    arr[1] = 0;

#pragma omp parallel \
    shared(arr) shared(sqrtMax) shared(max) private(base) private(next) 
    {
        # pragma omp for schedule(dynamic) 
        for(base = 2; base <= sqrtMax; base++){
            if(base < 0 || base > max) {
                PRINT0("Failing in first loop\n");
            }
            if(arr[base] == 1){
                //the real work starts here
#ifdef _OPENMP
                double wtime = omp_get_wtime();
#endif
                for(next = base + base; next < max; next += base){
                    if(next < 0 || next > max) {
                        PRINT0("Failing in second loop\n");
                        PRINT2("        Next is %lld, base is %lld\n", 
                            next, base);
                    }
                    arr[next] = 0;
                }
#ifdef _OPENMP
                wtime = omp_get_wtime() - wtime; 
                PRINT3("%f: time for base %4lld by thread num %2d\n",  
                    wtime, base, omp_get_thread_num());
#endif
            }
        }
    }
}

void printResult(char* arr, unsigned long long max){
    int i;
    for (i = 0; i < max; i++){
        if(arr[i] == 1){
            printf("%d\n", i);
        }
    }
    printf("\n");
}
