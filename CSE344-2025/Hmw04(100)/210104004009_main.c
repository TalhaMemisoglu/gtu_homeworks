#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "buffer.h"
 

// Global variables
Buffer *shared_buffer;
pthread_barrier_t barrier;
int num_workers;
char *search_term;
volatile sig_atomic_t terminate = 0;
int total_matches = 0;
pthread_mutex_t total_mutex = PTHREAD_MUTEX_INITIALIZER;
 
// Signal handler for SIGINT
void handle_sigint(int sig) {
    (void)sig;
    printf("\nTermination signal received. Cleaning up...\n");
    terminate = 1;
}
 
// Worker thread function
void *worker_thread(void *arg) {
    int worker_id = *((int *)arg);
    int matches_found = 0;
    char *line;
 
    printf("Worker %d started\n", worker_id);
 
    while (1) {
        // Check if termination requested
        if (terminate) {
            break;
        }
 
        // Consume from buffer
        line = buffer_get(shared_buffer);
         
        // Check for EOF marker
        if (line == NULL) {
            // Put EOF marker back for other workers and exit
            buffer_put(shared_buffer, NULL);
            break;
        }
 
        // Search for keyword
        if (strstr(line, search_term) != NULL) {
            matches_found++;
            printf("Worker %d found match: %s", worker_id, line);
        }
 
        // Free the line after processing
        free(line);
    }
 
    // Wait for all workers to finish before reporting
    pthread_barrier_wait(&barrier);
 
    // Report results
    if(!terminate) {
        printf("Worker %d found %d matches\n", worker_id, matches_found);
    }
    // Add worker's matches to the total count
    pthread_mutex_lock(&total_mutex);
    total_matches += matches_found;
    pthread_mutex_unlock(&total_mutex);
 
    // Only the last thread to reach the barrier prints the summary
    int result = pthread_barrier_wait(&barrier);
    if (result == PTHREAD_BARRIER_SERIAL_THREAD) {
        if(terminate) {
            printf("Termination signal received.\nWorkers haven't completed their search properly...\n");
            printf("Exiting...\n");
        } else if(!terminate) {
            printf("All workers have completed their search.\n");
            printf("\n--------- Summary ---------\n");
            printf("Search term: '%s'\n", search_term);
            printf("Total matches found: %d\n", total_matches);
            printf("---------------------------\n");
        }
    }
 
    free(arg);
    return NULL;
}
 
// Manager thread (main thread) function
void manager_thread(FILE *file) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
 
    // Read file line by line
    while ((read = getline(&line, &len, file)) != -1 && !terminate) {
        // Create a copy of the line
        char *line_copy = strdup(line);
        if (line_copy == NULL) {
            perror("Failed to allocate memory for line");
            continue;
        }
 
        // Put the line into the buffer
        buffer_put(shared_buffer, line_copy);
    }
 
    // Signal the end of file to worker threads
    buffer_put(shared_buffer, NULL);
 
    // Free the getline buffer
    if (line) {
        free(line);
    }
}
 
int main(int argc, char *argv[]) {
    // Set up signal handler for SIGINT
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);
    
    // Check for correct number of arguments
    if (argc != 5) {
        printf("Usage: %s <buffer_size> <num_workers> <log_file> <search_term>\n", argv[0]);
        return 1;
    }
 
    // Parse command line arguments
    int buffer_size = atoi(argv[1]);
    num_workers = atoi(argv[2]);
    char *file_path = argv[3];
    search_term = argv[4];
 
    // Validate arguments
    if (buffer_size <= 0 || num_workers <= 0) {
        printf("Buffer size and number of workers must be positive integers.\n");
        return 1;
    }
 
    // Open the log file
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Failed to open log file");
        return 1;
    }
 
    // Initialize shared buffer
    shared_buffer = buffer_init(buffer_size);
    if (shared_buffer == NULL) {
        perror("Failed to initialize buffer");
        fclose(file);
        return 1;
    }
 
    // Initialize barrier for worker threads
    if (pthread_barrier_init(&barrier, NULL, num_workers) != 0) {
        perror("Failed to initialize barrier");
        buffer_destroy(shared_buffer);
        fclose(file);
        return 1;
    }
 
    // Create worker threads
    pthread_t *workers = malloc(num_workers * sizeof(pthread_t));
    if (workers == NULL) {
        perror("Failed to allocate memory for worker threads");
        pthread_barrier_destroy(&barrier);
        buffer_destroy(shared_buffer);
        fclose(file);
        return 1;
    }
 
    printf("Starting log analysis with %d worker threads.\n", num_workers);
    printf("Buffer size: %d\n", buffer_size);
    printf("Search term: '%s'\n", search_term);
    printf("File: %s\n\n", file_path);
 
    // Start worker threads
    for (int i = 0; i < num_workers; i++) {
        int *id = malloc(sizeof(int));
        if (id == NULL) {
            perror("Failed to allocate memory for worker ID");
            continue;
        }
        *id = i + 1;
        pthread_create(&workers[i], NULL, worker_thread, id);
    }
 
    // Manager thread (main thread) reads the file
    manager_thread(file);
 
    // Wait for worker threads to finish
    for (int i = 0; i < num_workers; i++) {
        pthread_join(workers[i], NULL);
    }

    // Release all resources
    fclose(file);
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&total_mutex);
    buffer_destroy(shared_buffer);
    free(workers);
 
    return 0;
}