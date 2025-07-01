#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>

// Define satellite priority levels
#define LOW_PRIORITY 1    // Commercial
#define MEDIUM_PRIORITY 3 // Weather
#define HIGH_PRIORITY 5   // Military

// Number of engineers and satellites
#define NUM_ENGINEERS 3
#define NUM_SATELLITES 8

// Max connection window (in seconds)
#define MAX_CONNECTION_WINDOW 5

// Struct for satellite request
typedef struct {
    int id;
    int priority;
    pthread_t thread_id;
    sem_t request_handled;
    bool served;
} Satellite;

// Global variables
int availableEngineers = NUM_ENGINEERS;
Satellite* requestQueue[NUM_SATELLITES]; // Simple array to act as priority queue
int queueSize = 0;

// Synchronization primitives
pthread_mutex_t engineerMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t servedMutex = PTHREAD_MUTEX_INITIALIZER;
sem_t newRequest;

// Function prototypes
void* satellite(void* arg);
void* engineer(void* arg);
void push_request(Satellite* sat);
Satellite* pop_highest_priority();
void cleanup();

// Push satellite request to queue based on priority
void push_request(Satellite* sat) {
    requestQueue[queueSize] = sat;
    queueSize++;
    
    // Simple bubble sort to maintain priority order (highest first)
    for (int i = 0; i < queueSize - 1; i++) {
        for (int j = 0; j < queueSize - i - 1; j++) {
            if (requestQueue[j]->priority < requestQueue[j+1]->priority) {
                Satellite* temp = requestQueue[j];
                requestQueue[j] = requestQueue[j+1];
                requestQueue[j+1] = temp;
            }
        }
    }
}

// Pop highest priority satellite from queue
Satellite* pop_highest_priority() {
    if (queueSize == 0) {
        return NULL;
    }
    
    // Highest priority satellite is at the front
    Satellite* highestPriorityRequest = requestQueue[0];
    
    // Shift all other elements forward
    for (int i = 0; i < queueSize - 1; i++) {
        requestQueue[i] = requestQueue[i+1];
    }
    
    queueSize--;

    return highestPriorityRequest;
}

// Satellite thread function
void* satellite(void* arg) {
    Satellite* sat = (Satellite*)arg;
    printf("[SATELLITE] Satellite %d requesting...(Priority: %d)\n", sat->id, sat->priority);

    struct timespec start_time;
    clock_gettime(CLOCK_REALTIME, &start_time);
    
    // Initialize request_handled semaphore
    sem_init(&sat->request_handled, 0, 0);
    sat->served = false;

    // Acquire mutex to modify shared resources
    pthread_mutex_lock(&engineerMutex);

    // Add satellite to request queue
    push_request(sat);
    
    // Signal that a new request is available
    sem_post(&newRequest);
    
    // Release mutex
    pthread_mutex_unlock(&engineerMutex);

    // Set up timer for connection window
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += MAX_CONNECTION_WINDOW;
    
    // Wait for an engineer to handle the request, or timeout
    int result = sem_timedwait(&sat->request_handled, &timeout);

    pthread_mutex_lock(&servedMutex);
    if(sat->served || result == 0){
        // Request was handled by an engineer but not in time.
        //printf("[SATELLITE %d] update completed successfully.\n", sat->id);
        pthread_mutex_unlock(&servedMutex);
    } else if (result == -1 && errno == ETIMEDOUT) {
        pthread_mutex_unlock(&servedMutex);
        // Timeout occurred
        struct timespec end_time;
        clock_gettime(CLOCK_REALTIME, &end_time);

        double elapsed = (end_time.tv_sec - start_time.tv_sec) +
                 (end_time.tv_nsec - start_time.tv_nsec) / 1e9;


        pthread_mutex_lock(&engineerMutex);
        printf("[TIMEOUT] Satellite %d timed out after %.2f seconds\n", sat->id, elapsed);

        // Remove from queue if still there
        for (int i = 0; i < queueSize; i++) {
            if (requestQueue[i]->id == sat->id) {
                // Found the satellite, now remove it by shifting elements
                for (int j = i; j < queueSize - 1; j++) {
                    requestQueue[j] = requestQueue[j+1];
                }
                queueSize--;
                break;
            }
        }
        
        pthread_mutex_unlock(&engineerMutex);
    }
    // Clean up semaphore
    sem_destroy(&sat->request_handled);
    //free(sat);
    
    return NULL;
}

// Engineer thread function
void* engineer(void* arg) {
    int id = *((int*)arg);
    //printf("[ENGINEER %d] ready to serve.\n", id);
    
    while (1) {
        // Wait for a new request
        sem_wait(&newRequest);

        // Acquire mutex to modify shared resources
        pthread_mutex_lock(&engineerMutex);

        // Check if there are any requests in the queue
        Satellite* request = pop_highest_priority();
        
        if (request != NULL) {
            // Decrement availableEngineers
            availableEngineers--;
            printf("[ENGINEER %d] handling Satellite %d (Priority: %d)\n", 
                  id, request->id, request->priority);
            
            // Release mutex early so other engineers can work
            pthread_mutex_unlock(&engineerMutex);
            
            pthread_mutex_lock(&servedMutex);
            // Mark satellite as served
            request->served = true;
            pthread_mutex_unlock(&servedMutex);

            // Simulate processing time (1-3 seconds)
            sleep(4 + rand() % 3);
            
            // Signal that request has been handled
            sem_post(&request->request_handled);
            
            // Acquire mutex again to update availableEngineers
            pthread_mutex_lock(&engineerMutex);
            // Acquire mutex again to update availableEngineers
            availableEngineers++;
            printf("[ENGINEER %d] completed serving Satellite %d\n", 
                        id, request->id);
        }
        
        // Release mutex
        pthread_mutex_unlock(&engineerMutex);
    }
    
    return NULL;
}

// Cleanup function
void cleanup() {
    //Destroy semaphores and mutexes
    pthread_mutex_destroy(&servedMutex);
    pthread_mutex_destroy(&engineerMutex);
    sem_destroy(&newRequest);
}

int main() {
    // Seed random number generator
    srand(time(NULL));
    
    // Initialize semaphores
    sem_init(&newRequest, 0, 0);
    
    // Create satellites
    Satellite satellites[NUM_SATELLITES];
    pthread_t satelliteThreads[NUM_SATELLITES];
    
    // Create engineers
    pthread_t engineerThreads[NUM_ENGINEERS];
    int engineerIds[NUM_ENGINEERS];
    
    // Start engineer threads
    for (int i = 0; i < NUM_ENGINEERS; i++) {
        engineerIds[i] = i;
        pthread_create(&engineerThreads[i], NULL, engineer, &engineerIds[i]);
    }
    
    // Start satellite threads with random priorities
    for (int i = 0; i < NUM_SATELLITES; i++) {
        satellites[i].id = i;
        // Assign random priority (1, 3, or 5)
        int priorityValues[] = {LOW_PRIORITY, MEDIUM_PRIORITY, HIGH_PRIORITY};
        satellites[i].priority = priorityValues[rand() % 3];
        
        // Sleep for a short time to stagger satellite arrivals
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = (rand() % 500000) * 1000;  // convert to nanoseconds
        nanosleep(&ts, NULL);
        
        pthread_create(&satelliteThreads[i], NULL, satellite, &satellites[i]);
    }
    
    // Wait for all satellite threads to complete
    for (int i = 0; i < NUM_SATELLITES; i++) {
        pthread_join(satelliteThreads[i], NULL);
    }

    //Wait for a while to let engineers finish
    // This is just to ensure all engineers finish their work before exiting
    sleep(9);

    //Destroy engineer threads
    for (int i = 0; i < NUM_ENGINEERS; i++) {
        pthread_cancel(engineerThreads[i]);
        pthread_join(engineerThreads[i], NULL);
        printf("[ENGINEER %d] exiting...\n", i);
    }

    // Clean up resources
    cleanup();
    
    return 0;
}