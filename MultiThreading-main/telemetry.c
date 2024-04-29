//telemetry.c
#include "telemetry.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 1000
#define NUM_RUNNERS 10

// Define a global variable to indicate whether the race is ongoing
bool raceOngoing = true;

// Circular buffer to store telemetry data
TelemetryData telemetryBuffer[BUFFER_SIZE];
int head = 0;
int tail = 0;

// Definition of semaphores and mutex
sem_t buffer_empty;
sem_t buffer_full;
pthread_mutex_t buffer_mutex;

// Function to initialize semaphores and mutex
void initializeSemaphore() {
    sem_init(&buffer_empty, 0, BUFFER_SIZE);
    sem_init(&buffer_full, 0, 0);
    pthread_mutex_init(&buffer_mutex, NULL);
}

// Function to destroy semaphores and mutex
void destroySemaphore() {
    sem_destroy(&buffer_empty);
    sem_destroy(&buffer_full);
    pthread_mutex_destroy(&buffer_mutex);
}

// Implementation of the getData function
int getData(unsigned char *number, unsigned short *time, float *distance, float *speed) {
    // Simulation of the getData function
    *number = rand() % 100 + 1; // Runner rand between 1 and 100
    *time = rand() % 3600; // Time since the start in seconds (max 1 hour)
    *speed = (float)(rand() % 60); // Instantaneous speed in km/h (max 60 km/h)
    *distance = *speed * ((float)(*time) / 3600.0); // Distance covered since the start in km
    return 1; // Valid data
}

// Implementation of the writer thread
void *writerThread(void *arg) {
    while (raceOngoing) { // Loop until the race is ongoing
        // Get telemetry data
        unsigned char number;
        unsigned short time;
        float distance, speed;
        if (getData(&number, &time, &distance, &speed) <= 0) {
            continue; // Invalid data, move to the next cycle
        }

        // Exclusive access to the buffer
        sem_wait(&buffer_empty);
        pthread_mutex_lock(&buffer_mutex);

        // Write data to the buffer
        telemetryBuffer[head].number = number;
        telemetryBuffer[head].time = time;
        telemetryBuffer[head].distance = distance;
        telemetryBuffer[head].speed = speed;
        head = (head + 1) % BUFFER_SIZE;

        // Release the mutex and signal that the buffer is full
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&buffer_full);

        // Check if runner has completed 100km
        if (distance >= 100.0) {
            printf("Runner %d has completed 100km. Race terminated.\n", number);
            raceOngoing = false; // Signal the end of the race
        }

        // Wait for 1 millisecond
        usleep(1000);
    }

    return NULL;
}

// Implementation of the reader/writer thread to write to file
void *writeToFile(void *arg) {
    int runner_number = NUM_RUNNERS + *((int *)arg);

    while (raceOngoing) { // Loop until the race is ongoing
        // Wait for 1 second
        sleep(1);

        // Exclusive access to the buffer
        pthread_mutex_lock(&buffer_mutex);

        // Write data to file
        int current_tail = tail;
        while (current_tail != head) {
            TelemetryData data = telemetryBuffer[current_tail];
            if (data.number == (unsigned char)(runner_number)) {
                char filename[20];
                sprintf(filename, "runner_%d.csv", runner_number);
                FILE *file = fopen(filename, "a");
                if (file == NULL) {
                    perror("Error opening file");
                    exit(EXIT_FAILURE);
                }
                fprintf(file, "%d,%hu,%.2f,%.2f\n", data.number, data.time, data.distance, data.speed);
                fclose(file);
            }
            current_tail = (current_tail + 1) % BUFFER_SIZE;
        }

        // Release the mutex
        pthread_mutex_unlock(&buffer_mutex);
    }

    return NULL;
}
