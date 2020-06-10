#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cuda.h>
#include <curand.h>
#include <curand_kernel.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <ctype.h>
#include <sys/time.h>

//number of threads PER BLOCK
#define NTHREADS 1024
#define NBLOCKS 1024
#define TOTALTHREADS (NTHREADS*NBLOCKS)
// uncomment the below line to see output of prime numbers
#define DEBUG

void usage();
__global__ void Sieve(char* arr, unsigned long long max, unsigned long long start);
void printResult(char* arr, unsigned long long max);

int main(int argc, char** argv){
    unsigned long long max;
    int ret;
    char* array;
    char* cuda_array;

    //makes sure user has correct input
    if (argc != 2){
        usage();
    }
    max = atoll(argv[1]);
    //creates array of size user input on CPU
    array = (char*)malloc(sizeof(char) * max);
    if(array == NULL){
        printf("malloc failed\n");
        exit(1);
    }
    //creates array of size user input on gpu
    ret = cudaMalloc(&cuda_array, sizeof(char) * max);
    if (ret != cudaSuccess){
      printf("cudaMalloc of size %lld failed to return %d\n", max, ret);
      exit(1);
    }
    //memset all values to 1 which is the signature of being prime
    memset(array, 1, max);

    // 0 and 1 are not prime numbers
    array[0] = 0;
    array[1] = 0;

    //copy contents of CPU array into GPU array
    cudaMemcpy((void*)cuda_array, array, max, cudaMemcpyHostToDevice);
    unsigned long long sqrtMax;
    sqrtMax = (unsigned long long)(sqrt((double)max));
    /*can only summon so many threads at one so the for loop allows us to
     do so repetitevly*/
    for (unsigned long long i=0; i*TOTALTHREADS < sqrtMax; i++)
    {
      Sieve<<<NBLOCKS, NTHREADS>>>((char *)cuda_array, max, i*TOTALTHREADS);
    }


    //copy gpu array data to cpu array
    cudaMemcpy(array, (void*)cuda_array, max, cudaMemcpyDeviceToHost);

    #ifdef DEBUG
    printResult((char*)array, max);
    #endif
    //free gpu array and cpu array
    cudaFree((void*)cuda_array);
    free(array);

  }
//prints out correct usage if user does not provide proper input
void usage(){
    printf("usage: ./cudaSieve [maxInt]\n");
    exit(1);
}
__global__
void Sieve(char* arr, unsigned long long max, unsigned long long start){
    unsigned long long base = blockIdx.x * blockDim.x + threadIdx.x + start;
    unsigned long long next;
    unsigned long long sqrtMax;
    sqrtMax = (unsigned long long)sqrt((double)max);
    if (base > sqrtMax){
      return;
    }
    //check if base has been marked yet, if not then mark  off all muliples of base
    if(arr[base] == 1){
      for(next = base + base; next < max; next += base){
        arr[next] = 0;
      }
    }

}
//prints out result of numbers in interval that are prime
void printResult(char* arr, unsigned long long max){
    unsigned long long i;
    for (i = 0; i < max; i++){
        if(arr[i] == 1){
            printf("%lld\n", i);
        }
    }
    printf("\n");
}
