#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void usage();
void Sieve(char* arr, int max);
void printResult(char* arr, int max);

int main(int argc, char** argv){
    int max;
    char* array;
    if (argc != 2){
        usage();
    }
    max = atoi(argv[1]);
    array = malloc(sizeof(char) * max);
    if(array == NULL){
        printf("malloc failed\n");
        exit(1);
    }
    memset(array, 1, max);
    Sieve(array, max);
    printResult(array, max);
    free(array);
}

void usage(){
    printf("usage: ./sequentialSieve [maxInt]\n");
    exit(1);
}

void Sieve(char* arr, int max){
    int base;
    int next;
    double sqrtMax;
    sqrtMax = sqrt((double)max);
    arr[0] = 0;
    arr[1] = 0;
    for(base = 2; base < sqrtMax; base++){
        if(arr[base] == 1){
            for(next = base + base; next < max; next += base){
                arr[next] = 0;
            }
        }
    }
}

void printResult(char* arr, int max){
    int i;
    for (i = 0; i < max; i++){
        if(arr[i] == 1){
            printf("%d\n", i);
        }
    }
    printf("\n");
}
