#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2

// Function executed by the threads
void *threadFunction(void *threadId) {
    long tid = (long)threadId;

    // This code is executed concurrently
    printf("Thread %ld: Hello, concurrent world!\n", tid);

    // This code is executed concurrently
    printf("Thread %ld: Goodbye, concurrent world!\n", tid);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // This code is executed sequentially in the main thread
    printf("Main thread: Hello from the main thread!\n");

    // Create threads
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, threadFunction, (void *)t);
        if (rc) {
            printf("Error: Unable to create thread %ld\n", t);
            exit(-1);
        }
    }

    // Wait for threads to finish
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // This code is executed sequentially in the main thread
    printf("Main thread: Goodbye from the main thread!\n");

    pthread_exit(NULL);
}
