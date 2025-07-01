#define _POSIX_C_SOURCE 200809L

#ifndef BUFFER_H
#define BUFFER_H
 
// Buffer structure
typedef struct {
    char **data;        // Array of line pointers
    int size;           // Maximum buffer size
    int count;          // Current number of items
    int in;             // Position for next put
    int out;            // Position for next get
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} Buffer;
 
 // Function declarations
Buffer *buffer_init(int size);
void buffer_destroy(Buffer *buffer);
void buffer_put(Buffer *buffer, char *line);
char *buffer_get(Buffer *buffer);
 
#endif /* BUFFER_H */