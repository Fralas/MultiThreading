// main.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "telemetry.h"

#define NUM_THREADS 10

int main() {
    // Initialization of semaphores and mutex
    initializeSemaphore();

    // Creation of writer thread
    pthread_t writer;
    pthread_create(&writer, NULL, writerThread, NULL);

    // Creation of reader/writer threads
    pthread_t readers[NUM_THREADS];
    int *thread_args[NUM_THREADS]; // Array to store thread arguments
    for (int i = 0; i < NUM_THREADS; i++) {
        // Allocate memory for thread argument
        thread_args[i] = (int *)malloc(sizeof(int));
        *thread_args[i] = i; // Store loop variable i in thread argument
        // Create thread
        pthread_create(&readers[i], NULL, writeToFile, (void *)thread_args[i]);
    }

    // Waits for threads to terminate or race completion
    pthread_join(writer, NULL);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(readers[i], NULL);
        // Free allocated memory for thread argument
        free(thread_args[i]);
    }

    // Destruction of semaphores and mutex
    destroySemaphore();

    return 0;
}
