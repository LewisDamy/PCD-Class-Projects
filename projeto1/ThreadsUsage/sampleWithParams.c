#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3

// Define a structure to hold thread parameters
struct ThreadParams {
    int threadId;
    char *message;
};

// Function executed by the threads
void *threadFunction(void *param) {
    struct ThreadParams *params = (struct ThreadParams *)param;

    // This code is executed concurrently
    printf("Thread %d: Received message: %s\n", params->threadId,
           params->message);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    struct ThreadParams params[NUM_THREADS];

    // Create threads with different parameters
    for (int t = 0; t < NUM_THREADS; t++) {
        params[t].threadId = t;
        params[t].message = t == 0 ? "Hello" : t == 1 ? "Bonjour" : "Hola";

        rc = pthread_create(&threads[t], NULL, threadFunction,
                            (void *)&params[t]);
        if (rc) {
            printf("Error: Unable to create thread %d\n", t);
            exit(-1);
        }
    }

    // Wait for threads to finish
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_exit(NULL);
}
