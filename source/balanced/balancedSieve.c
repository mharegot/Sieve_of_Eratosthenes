/* An implementation of parallel sieve of Eratosthenes using a balanced
 * pthreads approach
 * CS87 group project Swarthmore College 2020
 * Kevin Zheng, Rick Muniu, Mickey Haregot
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#define NONPRIME 1
#define PRIME 0

//#define DEBUG

// a package of data passed to every thread on startup
typedef struct {
    unsigned long long size;
    int numthreads;
    pthread_t threadid;
    int tid;
    char* arr;
    unsigned long long* base;
} package;

void usage();
void* threadStart(void* inbox);
package* launchThreads(int argc, char** argv);
void joinThreads(package* postoffice);
void findBounds(package* mail, unsigned long long *start, unsigned long long *loclen);
void printResults(unsigned long long size, char* arr);
void cleanState(package* postoffice);
void initArr(package* mail, unsigned long long start, unsigned long long loclen);
void masterLoop(package* mail, unsigned long long start, unsigned long long loclen);
void slaveLoop(package* mail, unsigned long long start, unsigned long long loclen);

static pthread_barrier_t basebarrier;

int main(int argc, char** argv){
    // initialize and launch all threads. Their info is found in the
    // postoffice array
    package* postoffice;
    postoffice = launchThreads(argc, argv);

    // join threads back together when alg finishes
    joinThreads(postoffice);

    // print all primes found
#ifdef DEBUG
    printResults(postoffice[0].size, postoffice[0].arr);
#endif

    cleanState(postoffice);
    return 0;
}

// simple print to be shown when user enters invalid input
void usage(){
    printf("usage: ./bal_sieve [max] [nthreads]\n");
    printf("\t@max: highest number to search to\n");
    printf("\t@nthreads: number of threads to run with\n");
    exit(1);
}

/* Initalizes and launches all threads based on command line arguments
 * Returns a completed package* array that has the basic info
 * for every thread, including the pthread_t values
 * that will later be needed to join the threads
 *
 * This function does a lot of work. Goes from raw user input to launched 
 * threads
 */
package* launchThreads(int argc, char** argv){
    unsigned long long size;
    unsigned int nthreads;
    char* arr;
    unsigned long long* base;
    int i;
    int ret;
    package* postoffice;

    // parse input arguments for the size of the array
    if(argc != 3){
        usage();
    }
    size = atoll(argv[1]);
    if(size == 0){
        usage();
    }

    nthreads = atoi(argv[2]);
    if(nthreads == 0){
        usage();
    }

    // allocate space for main array
    arr = malloc(sizeof(char)*size);
    if(arr == NULL){
        printf("Malloc failed\n");
        exit(1);
    }

    // allocate space for shared base variable
    base = malloc(sizeof(unsigned long long));
    if(base == NULL){
        printf("Malloc failed\n");
        exit(1);
    }
    *base = 0;

    // allocate space for package structs
    postoffice = malloc(sizeof(package)*nthreads);
    if(postoffice == NULL){
        printf("Malloc failed\n");
        exit(1);
    }

    // initalize barrier
    if(pthread_barrier_init(&basebarrier, 0, nthreads)){
        perror("pthread_barrier_init error");
        exit(1);
    }

    // initialize each package with basic info and call pthread_create
    // to kick off the thread execution
    for(i = 0; i < nthreads; i++){
        postoffice[i].size = size;
        postoffice[i].tid = i;
        postoffice[i].arr = arr;
        postoffice[i].numthreads = nthreads;
        postoffice[i].base = base;
        ret = pthread_create(&postoffice[i].threadid, NULL, threadStart,
                &postoffice[i]);
        if(ret != 0){
            perror("pthread_create error");
            exit(1);
        }
    }
    return postoffice;
}

/* Joins all the threads when the algorithm
 * is finished running
 * postoffice is not cleaned up: it still contains pointer to grid
 * which needs to be used to report results
 */
void joinThreads(package* postoffice){
    int ret;
    int i;
    int numthreads = postoffice[0].numthreads;
    for(i = 0; i < numthreads; i++){
        ret = pthread_join(postoffice[i].threadid, NULL);
        if(ret != 0){
            perror("pthread_create error");
            exit(1);
        }
    }
}

/* given the finished array and its size, prints out the prime numbers
 * that were found, each one being separated by whitespace
 */
void printResults(unsigned long long size, char* arr){
    unsigned long long i;
    for(i = 0; i < size; i++){
        if(arr[i] == PRIME){
            printf("%lld\n", i);
        }
    }
}

/* Cleans up memory and resources associated with this run,
 * including the postoffice array, the char array we were using to find
 * the primes, and the pthread barrier resources
 */
void cleanState(package* postoffice){
    // clean up barrier
    pthread_barrier_destroy(&basebarrier);
    // clean up memory
    free(postoffice[0].base);
    free(postoffice[0].arr);
    free(postoffice);
}

/* Marks the start of the execution point for each thread. Each thread
 * will find its own bounds, initialize its own portion of the board
 * and synchronize with each other for each round where the master thread
 * finds a base number
 */
void* threadStart(void* inbox){
    unsigned long long start;
    unsigned long long loclen;
    package* mail = (package*)inbox;

    // find section of the array this thread is responsible for
    findBounds(mail, &start, &loclen);

#ifdef DEBUG
    printf("thread: %d, start: %lld, end: %lld\n", mail->tid, start, start+loclen);
    fflush(stdout);
#endif

    initArr(mail, start, loclen);

    if(mail->tid == 0){
        masterLoop(mail, start, loclen);
    }
    else{
        slaveLoop(mail, start, loclen);
    }

    return NULL;
}

/* Each thread determines which part of the array it is responsible
 * for. The 0th thread is the master and takes from 0 to sqrt(N), and
 * all other threads evenly split the remainder of the array
 * Each thread is responsible from index start(inclusive) up to index end
 * (not inclusive)
 */
void findBounds(package* mail, unsigned long long *start, unsigned long long *loclen){
    int extracounter;
    int i;
    // calculate remaining size, start, and the standard jump after
    // accounting for the master thread
    unsigned long long masterlen = sqrt(mail->size) + 1;
    unsigned long long remsize = mail->size - masterlen;
    unsigned long long remstart = masterlen;
    unsigned long long stdjump = remsize / (mail->numthreads - 1);
    // calculate how many threads will need to be responsible
    // for one extra element
    extracounter = remsize % (mail->numthreads - 1);
    *start = remstart;

    // master thread special case
    if(mail->tid == 0){
        *start = 0;
        *loclen = masterlen;
        return;
    }

    // calculate how far to jump based on which tid this thread
    // is assigned
    for(i = 1; i < mail->tid; i++){

        // usually jump by stdjump, but jump by one more for the threads
        // that need to be responsible for one more
        *start += stdjump;
        if(extracounter){
            extracounter--;
            *start += 1;
        }

        // if a previous thread already has covered up to the end
        // of the array, then this thread has nothing more to do
        if(*start >= mail->size){
            *start = mail->size;
            *loclen = 0;
            return;
        }
    }
    // end is one stdjump away from start, but may need one extra based
    // on extracounter
    *loclen = stdjump;
    if(extracounter){
        *loclen += 1;
    }
    return;
}

/* Each thread calls this function to initialize its part of the
 * array to be completely unmarked (that is, initially assumed to be prime)
 */
void initArr(package* mail, unsigned long long start, unsigned long long loclen){
    unsigned long long i;
    for(i = 0; i < loclen; i++){
        // All numbers start unmarked
        mail->arr[start + i] = PRIME;
    }
}

/* This is the main execution loop of the master thread. It uses sequential
 * Sieve of Eratosthenes to find bases which it broadcasts to all slave
 * threads using a barrier and a shared variable
 */
void masterLoop(package* mail, unsigned long long start, unsigned long long loclen){
    unsigned long long base;
    unsigned long long next;

    // 0 and 1 are not prime numbers
    mail->arr[0] = NONPRIME;
    mail->arr[1] = NONPRIME;

    // loop in which we look for new bases to investigate
    for(base = 2; base < loclen; base++){
        // check to see if a base is unmarked and worth investigating
        if(mail->arr[base] == PRIME){
            // once a base is found, broadcast it to all slaves
            pthread_barrier_wait(&basebarrier);
            *(mail->base) = base;
            pthread_barrier_wait(&basebarrier);

            // just like all slaves, master must also mark off all multiples
            // of the base within its own section of the array
            for(next = base + base; next < loclen; next += base){
                mail->arr[next] = NONPRIME;
            }
        }
    }
    // when algorithm is done, broadcast 0 to all threads and exit
    pthread_barrier_wait(&basebarrier);
    *(mail->base) = 0;
    pthread_barrier_wait(&basebarrier);
    return;
}

/* This is the main execution loop of a slave thread. It waits for notification
 * from master thread, then finds all multiples of the broadcasted base
 * within its portion of the array, marking off those spots in the array
 */
void slaveLoop(package* mail, unsigned long long start, unsigned long long loclen){
    unsigned long long base;
    unsigned long long next;
    unsigned long long disfromstart;

    while(1){
        // first barrier wait is triggered when all slaves are done with
        // the previous base and the master has found a new one
        pthread_barrier_wait(&basebarrier);

        // second barrier wait is triggered when master has finished
        // writing the new base to shared memory. It is only safe to read
        // after this barrier
        pthread_barrier_wait(&basebarrier);
        base = *(mail->base);
        
        // master broadcasts 0 to all threads if it is time to quit
        if(base == 0){
            return;
        }
        
        // first step: identify the first multiple of the base within
        // the array section
        disfromstart = start % base;
        if(disfromstart == 0){
            next = start;
        }
        else{
            next = start + (base - disfromstart);
        }

        // second step: use the first multiple to find all other multiples
        // of the base within the array section
        for(next = next; next < start + loclen; next += base){
            mail->arr[next] = NONPRIME;
        }
    }
}
