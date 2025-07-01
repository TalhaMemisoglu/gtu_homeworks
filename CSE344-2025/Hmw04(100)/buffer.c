#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "buffer.h"
 
/**
* Initialize a new buffer
* 
* @param size Size of the buffer
* @return Pointer to the new buffer, or NULL on failure
*/
Buffer *buffer_init(int size) {
    Buffer *buffer = malloc(sizeof(Buffer));
    if (buffer == NULL) {
        return NULL;
    }
 
    buffer->data = malloc(size * sizeof(char *));
    if (buffer->data == NULL) {
        free(buffer);
        return NULL;
    }
 
    buffer->size = size;
    buffer->count = 0;
    buffer->in = 0;
    buffer->out = 0;
 
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->not_full, NULL);
    pthread_cond_init(&buffer->not_empty, NULL);
 
    return buffer;
}
 
/**
* Clean up and free the buffer
* 
* @param buffer Buffer to destroy
*/
void buffer_destroy(Buffer *buffer) {
    if (buffer == NULL) {
        return;
    }
 
    // Free any remaining lines in the buffer
    for (int i = 0; i < buffer->count; i++) {
        int index = (buffer->out + i) % buffer->size;
        if (buffer->data[index] != NULL) {
            free(buffer->data[index]);
        }
    }
 
    free(buffer->data);
     
    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->not_full);
    pthread_cond_destroy(&buffer->not_empty);
     
    free(buffer);
}
 
/**
* Put a line into the buffer
* 
* @param buffer Buffer to operate on
* @param line Line to add (or NULL as EOF marker)
*/
void buffer_put(Buffer *buffer, char *line) {
    pthread_mutex_lock(&buffer->mutex);
 
    // Wait until there is space in the buffer
    while (buffer->count == buffer->size) {
        pthread_cond_wait(&buffer->not_full, &buffer->mutex);
    }
 
    // Put the line in the buffer
    buffer->data[buffer->in] = line;
    buffer->in = (buffer->in + 1) % buffer->size;
    buffer->count++;
 
    // Signal that the buffer is not empty
    pthread_cond_signal(&buffer->not_empty);
    pthread_mutex_unlock(&buffer->mutex);
}
 
/**
* Get a line from the buffer
* 
* @param buffer Buffer to operate on
* @return The next line from the buffer, or NULL for EOF
*/
char *buffer_get(Buffer *buffer) {
    pthread_mutex_lock(&buffer->mutex);
 
    // Wait until there is data in the buffer
    while (buffer->count == 0) {
        pthread_cond_wait(&buffer->not_empty, &buffer->mutex);
    }
 
    // Get the line from the buffer
    char *line = buffer->data[buffer->out];
    buffer->out = (buffer->out + 1) % buffer->size;
    buffer->count--;
 
    // Signal that the buffer is not full
    pthread_cond_signal(&buffer->not_full);
    pthread_mutex_unlock(&buffer->mutex);
 
    return line;
}