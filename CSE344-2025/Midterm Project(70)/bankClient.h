
#ifndef BANKCLIENT_H
#define BANKCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>


#define MAX_CLIENTS 100
#define MAX_BUFFER_SIZE 256
#define MAX_PATH_LENGTH 100

// Transaction types
#define DEPOSIT 'D'
#define WITHDRAW 'W'

typedef struct {
    char account_id[20];  // Account ID or "N" for new accounts
    char operation;       // 'D' for deposit, 'W' for withdraw
    int amount;           // Transaction amount
    int status;           // Success/failure status code
    char message[100];    // Status message
} Request;

typedef struct {
    int client_num;
    char account_id[20];
    Request transaction;
    int connected;
} ClientInfo;

// Function prototypes
void initialize_client(const char* server_fifo);
void cleanup_client();
void handle_signals(int signo);
int read_client_file(const char* filename);
int connect_to_server();
void create_client_processes();
void client_process(ClientInfo* client);

// Global variables
extern char server_fifo_name[MAX_PATH_LENGTH];
extern ClientInfo clients[MAX_CLIENTS];
extern int client_count;
extern int running;

#endif // BANKCLIENT_H