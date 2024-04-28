#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <semaphore.h>
#include <pthread.h>

// Definition of telemetry data
typedef struct {
    unsigned char number;
    unsigned short time;
    float distance;
    float speed;
} TelemetryData;

// Declaration of semaphores and mutex
extern sem_t buffer_empty;
extern sem_t buffer_full;
extern pthread_mutex_t buffer_mutex;

// Functions
void initializeSemaphore();
void destroySemaphore();
int getData(unsigned char *number, unsigned short *time, float *distance, float *speed);
void *writerThread(void *arg);
void *writeToFile(void *arg);

#endif
