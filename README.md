**Telemetry System README**

*Overview*
The telemetry system provided in this code is designed to collect and store data from a simulated race, including the number of each runner, the time since the start of the race, the distance covered, and the instantaneous speed of each runner. This telemetry data is then written to files for further analysis.


*Components*
telemetry.h 
This header file contains declarations for functions and structures used in the telemetry system. It includes the following:

Definition of TelemetryData structure to store telemetry information for each runner.
Declaration of semaphores (buffer_empty and buffer_full) and a mutex (buffer_mutex) used for thread synchronization.
Function prototypes for initializing and destroying semaphores, getting telemetry data, and two thread functions.
telemetry.c
This source file contains the implementation of functions declared in telemetry.h. Key components include:

Circular buffer (telemetryBuffer) to store telemetry data.
Implementation of initializeSemaphore() and destroySemaphore() to initialize and destroy semaphores and mutex.
Implementation of getData() to simulate getting telemetry data.
Implementation of writerThread() to continuously collect telemetry data and write it to the circular buffer.
Implementation of writeToFile() to periodically write telemetry data to files for each runner.


*main.c*
This source file contains the main() function where the telemetry system is orchestrated. It includes:

Initialization of semaphores and mutex.
Creation of a writer thread (writerThread) to collect telemetry data.
Creation of multiple reader/writer threads (writeToFile) to periodically write telemetry data to files for each runner.
Waiting for threads to terminate or race completion.
Destruction of semaphores and mutex.


*How it Works*
Initialization: Semaphores and mutex are initialized using initializeSemaphore() before starting the threads.
Writer Thread: The writerThread continuously collects telemetry data from a simulated race and writes it to the circular buffer.
Reader/Writer Threads: Multiple reader/writer threads are created to periodically write telemetry data from the circular buffer to files for each runner.
Race Termination: The system detects when a runner completes 100 km, signaling the end of the race.
Thread Synchronization: Semaphores and mutex are used for thread synchronization to ensure safe access to shared resources (circular buffer).
Cleanup: After the race is completed, semaphores and mutex are destroyed using destroySemaphore().


*Usage*
To use this telemetry system:
Include telemetry.h in your project.
Make sure to have a csv file for each runner located in the same directory as the main.c file (Ex: runner_1.csv // runner_2.csv)