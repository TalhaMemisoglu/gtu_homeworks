#ifndef BANKSERVER_H
#define BANKSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/select.h>
#include <ctype.h>

#define MAX_ACCOUNTS 100
#define MAX_BUFFER_SIZE 256
#define MAX_PATH_LENGTH 100
#define MAX_CLIENTS 100
#define MAX_TRANSACTION 100

// Transaction types
#define DEPOSIT 'D'
#define WITHDRAW 'W'

// Error codes
#define SUCCESS 0
#define ERR_INSUFFICIENT_FUNDS 1
#define ERR_INVALID_ACCOUNT 2
#define ERR_INVALID_OPERATION 3
#define ERR_SYSTEM_ERROR 4

// Semaphore operations
#define SEM_LOCK -1
#define SEM_UNLOCK 1

typedef struct {
    char operation;          // 'D' for deposit, 'W' for withdraw
    int amount;              // Transaction amount
} Transaction;

typedef struct {
    int account_id;                        // e.g., "1"
    int balance;                                // Current balance
    Transaction transactions[MAX_TRANSACTION];  // Pointer to transaction history
    int transaction_count;                      // Number of transactions
} BankAccount;

typedef struct {
    char account_id[20];  // Account ID or "N" for new accounts
    char operation;       // 'D' for deposit, 'W' for withdraw
    int amount;           // Transaction amount
    int status;           // Success/failure status code
    char message[100];    // Status message
} Request;

typedef struct {
    BankAccount accounts[MAX_ACCOUNTS];
    int account_count;
    int next_account_id;
    char log_filename[108];
    time_t last_update;
} BankDatabase;

// Shared memory structure for Teller-Server communication
typedef struct {
    // Request from teller to server
    int request_ready;
    Request request;
    
    // Response from server to teller
    int response_ready;
    Request response;
} SharedMemorySegment;

// Custom Teller process creation function
pid_t Teller(void* func, void* arg_func);

// Custom wait function for Teller processes
int waitTeller(pid_t pid, int* status);

// Function pointers for deposit and withdraw operations
typedef int (*OperationFunc)(void*);

// Function prototypes for server
void initialize_server(const char* bank_name, const char* server_fifo_name);
void initialize_database();
void cleanup_server();
void handle_signals(int signo);
void process_client_connections();
void process_teller_requests();
void create_teller(int client_pid, int client_no, char client_operation);
void update_log_file();
void load_database_from_log();

int is_valid_bank_id(const char* bank_id);

// Function prototypes for transaction handling
int process_transaction(Request* transaction);
int deposit(void* args);
int withdraw(void* args);
BankAccount* find_account(const char* account_id);

// Pipe handling functions
int open_server_fifo(const char* fifo_name, int mode);

// Semaphore functions
int init_semaphore();
void sem_operation(int sem_id, int op);
void cleanup_semaphore(int sem_id);

// Shared memory functions
int create_shared_memory();
SharedMemorySegment* attach_shared_memory(int shm_id);
void detach_shared_memory(SharedMemorySegment* shm);
void cleanup_shared_memory(int shm_id);

// Global variables
extern BankDatabase bank_db;
extern char bank_name[MAX_PATH_LENGTH];
extern char server_fifo_name[MAX_PATH_LENGTH];
extern int server_fifo_fd;
extern int running;
extern pid_t teller_pids[MAX_CLIENTS];
extern int teller_count;

#endif // BANK_SERVER_H