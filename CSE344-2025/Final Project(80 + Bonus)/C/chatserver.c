#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdarg.h>
#include <fcntl.h>

#define MAX_CLIENTS 50
#define MAX_ROOMS 20
#define MAX_ROOM_CAPACITY 15
#define MAX_USERNAME_LEN 17
#define MAX_ROOMNAME_LEN 33 
#define MAX_MSG_SIZE 4096
#define MAX_CMD_SIZE 256
#define MAX_UPLOAD_QUEUE 5
#define MAX_FILE_SIZE 3*1024*1024 // 3MB
#define BUFFER_SIZE 1024
#define LOG_BUFFER_SIZE 4096
#define FILE_TRANSFER_BUFFER 1024

// Valid file extensions
const char* VALID_EXTENSIONS[] = {".txt", ".pdf", ".jpg", ".png"};
const int NUM_VALID_EXTENSIONS = 4;

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

// Global flag for graceful shutdown
volatile sig_atomic_t keep_running = 1;

// Client structure
typedef struct {
    int socket;
    char username[MAX_USERNAME_LEN];
    char ip_address[INET_ADDRSTRLEN];
    char current_room[MAX_ROOMNAME_LEN];
    bool connected;
    pthread_t thread;
    char previous_rooms[MAX_ROOMS][MAX_ROOMNAME_LEN];  // Track rooms user has been in
    int num_previous_rooms;
    bool file_transfer_in_progress;  // Flag to indicate file transfer mode
    pthread_mutex_t client_mutex;    // Mutex for client state
} client_t;

typedef struct {
    char name[MAX_ROOMNAME_LEN];
    int num_clients;
    client_t *clients[MAX_ROOM_CAPACITY];
    pthread_mutex_t room_mutex;
    bool active;
} room_t;

typedef struct {
    char filename[256];
    char sender[MAX_USERNAME_LEN];
    char recipient[MAX_USERNAME_LEN];
    time_t request_time;
    time_t start_time;  // When transfer actually starts
    bool in_progress;
    int sender_socket;
    size_t file_size;
} file_transfer_t;

// Global variables
int server_socket;
int signal_pipe[2];  // Self-pipe for signal handling
client_t *clients[MAX_CLIENTS];
room_t rooms[MAX_ROOMS];
file_transfer_t upload_queue[MAX_UPLOAD_QUEUE];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rooms_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t upload_semaphore;

FILE *log_file;

// Add a new global variable for worker thread synchronization
pthread_cond_t worker_ready_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t worker_ready_mutex = PTHREAD_MUTEX_INITIALIZER;
bool worker_thread_ready = false;

// Add condition variable for file transfer queue
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

void write_log(const char *format, ...);
void handle_client(void *arg);
int find_client_by_username(const char *username);
int add_client(int client_socket, const char *username, const char *ip);
void remove_client(int client_idx);
int create_or_join_room(int client_idx, const char *room_name);
void leave_room(int client_idx);
void broadcast_message(int client_idx, const char *message);
void whisper_message(int client_idx, const char *recipient, const char *message);
int add_to_upload_queue(const char *filename, const char *sender, const char *recipient, int sender_socket, size_t file_size);
int get_queue_position(const char *sender, const char *filename);
void* file_transfer_worker(void *arg);
void process_command(int client_idx, char *command);
bool is_valid_username(const char *username);
bool is_valid_roomname(const char *roomname);
bool is_valid_file_extension(const char *filename);
void handle_sigint(int sig);
void cleanup_and_exit();
void send_message(int client_socket, const char *message);
void setup_server_directories();
char* generate_unique_filename(const char* directory, const char* filename);
bool has_been_in_room(int client_idx, const char* room_name);
void add_room_to_history(int client_idx, const char* room_name);


int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    // Install signal handler
    signal(SIGINT, handle_sigint);

    // Initialize server
    int port = atoi(argv[1]);
    int client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t file_worker_thread;

    // Create log directory and file
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char log_path[256];

    //Create logs directory if it doesn't exist
    if (mkdir("logs", 0777) == -1) 
    {
        if (errno != EEXIST) {
            perror("Failed to create logs directory");
            return 1;
        }
    }

    sprintf(log_path, "logs/server_log_%04d-%02d-%02d_%02d-%02d-%02d.log", 
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, 
        t->tm_hour, t->tm_min, t->tm_sec);

    log_file = fopen(log_path, "w");
    if (!log_file) 
    {
        perror("Failed to open log file");
        return 1;
    }

    write_log("Server starting on port %d", port);

    // Initialize client array
    memset(clients, 0, sizeof(clients));
    
    // Initialize rooms
    for (int i = 0; i < MAX_ROOMS; i++) 
    {
        rooms[i].active = false;
        pthread_mutex_init(&rooms[i].room_mutex, NULL);
    }
    
    // Initialize upload queue
    memset(upload_queue, 0, sizeof(upload_queue));
    sem_init(&upload_semaphore, 0, MAX_UPLOAD_QUEUE);
    
    // Create self-pipe for signal handling
    if (pipe(signal_pipe) == -1) 
    {
        write_log("ERROR: Failed to create signal pipe");
        perror("Failed to create signal pipe");
        fclose(log_file);
        return 1;
    }
    
    // Make write end non-blocking
    int flags = fcntl(signal_pipe[1], F_GETFL);
    if (flags != -1) 
    {
        fcntl(signal_pipe[1], F_SETFL, flags | O_NONBLOCK);
    }
    
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) 
    {
        write_log("ERROR: Failed to create socket");
        perror("Failed to create socket");
        fclose(log_file);
        return 1;
    }
    
    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        write_log("ERROR: setsockopt failed");
        perror("setsockopt failed");
        fclose(log_file);
        return 1;
    }
    
    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        write_log("ERROR: Bind failed");
        perror("Bind failed");
        fclose(log_file);
        return 1;
    }
    
    // Listen for connections
    if (listen(server_socket, 10) < 0) 
    {
        write_log("ERROR: Listen failed");
        perror("Listen failed");
        fclose(log_file);
        return 1;
    }
    
    printf("%s=== CHAT SERVER STARTED ===%s\n", COLOR_GREEN, COLOR_RESET);
    printf("Server started on port %d\n", port);
    printf("%sListening for connections...%s\n\n", COLOR_CYAN, COLOR_RESET);
    write_log("Server listening on port %d", port);
    
    // Create directory for file uploads
    setup_server_directories();

    // Create file transfer worker thread
    if (pthread_create(&file_worker_thread, NULL, file_transfer_worker, NULL) != 0) 
    {
        write_log("ERROR: Failed to create file worker thread");
        perror("Failed to create file worker thread");
        fclose(log_file);
        return 1;
    }
    
    // Wait for the file worker thread to be ready before accepting connections
    pthread_mutex_lock(&worker_ready_mutex);
    while (!worker_thread_ready) {
        pthread_cond_wait(&worker_ready_cond, &worker_ready_mutex);
    }
    pthread_mutex_unlock(&worker_ready_mutex);
    
    // Detach the file worker thread so it can clean up its own resources
    pthread_detach(file_worker_thread);

    // Main server loop
    while (keep_running) 
    {
        fd_set read_fds;
        
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        FD_SET(signal_pipe[0], &read_fds);  // Monitor signal pipe
        
        int max_fd = (server_socket > signal_pipe[0]) ? server_socket : signal_pipe[0];
        
        // Block indefinitely until activity - no more polling!
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        
        if (activity < 0) 
        {
            if (errno != EINTR) {
                perror("Select error");
            }
            continue;
        }
        
        // Check for signal received
        if (FD_ISSET(signal_pipe[0], &read_fds)) 
        {
            char buffer[256];
            read(signal_pipe[0], buffer, sizeof(buffer));  // Drain the pipe
            
            printf("\n%s=== SERVER SHUTTING DOWN ===%s\n", COLOR_YELLOW, COLOR_RESET);
            keep_running = 0;
            break;
        }
        
        if (FD_ISSET(server_socket, &read_fds)) 
        {
            client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
            
            if (!keep_running) {
                if (client_socket > 0) close(client_socket);
                break;
            }
            
            if (client_socket < 0) {
                write_log("ERROR: Accept failed");
                perror("Accept failed");
                continue;
            }
            
            // Get client IP address
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
            printf("New connection from %s:%d\n", client_ip, ntohs(client_addr.sin_port));
            
            // Username validation loop - allow multiple attempts
            bool username_accepted = false;
            char username[MAX_USERNAME_LEN];
            
            while (!username_accepted && keep_running) 
            {
                // Receive username
                ssize_t bytes_received = recv(client_socket, username, sizeof(username) - 1, 0);
                if (bytes_received <= 0) 
                {
                    close(client_socket);
                    break;
                }
                username[bytes_received] = '\0';
                
                // Validate username format
                if (!is_valid_username(username)) 
                {
                    char *msg = "ERROR: Invalid username. Use only alphanumeric characters (max 16)";
                    send(client_socket, msg, strlen(msg), 0);
                    write_log("[REJECTED] Invalid username format: %s from %s", username, client_ip);
                    continue;  // Ask for username again
                }
                
                // Check if username is already taken
                pthread_mutex_lock(&clients_mutex);
                bool username_taken = false;
                for (int i = 0; i < MAX_CLIENTS; i++) 
                {
                    if (clients[i] != NULL && strcmp(clients[i]->username, username) == 0) {
                        username_taken = true;
                        break;
                    }
                }
                pthread_mutex_unlock(&clients_mutex);
                
                if (username_taken) 
                {
                    char *msg = "ERROR: Username already taken. Please try another username.";
                    send(client_socket, msg, strlen(msg), 0);
                    write_log("[REJECTED] Duplicate username attempted: %s from %s", username, client_ip);
                    continue;  // Ask for username again
                }
                
                // Username is valid and available
                username_accepted = true;
            }
            
            if (!username_accepted) 
            {
                close(client_socket);
                continue;
            }
            
            // Add client to the array
            int client_idx = add_client(client_socket, username, client_ip);
            if (client_idx == -1) 
            {
                char *msg = "ERROR: Server is full. Try again later.";
                send(client_socket, msg, strlen(msg), 0);
                close(client_socket);
                write_log("REJECTED: Server full. Connection from %s rejected", client_ip);
                continue;
            }
            
            // Send welcome message
            char welcome_msg[256];
            sprintf(welcome_msg, "SUCCESS: Welcome to the chat server, %s!", username);
            send(client_socket, welcome_msg, strlen(welcome_msg), 0);
            
            printf("%s[LOGIN]%s User '%s' connected from %s\n", COLOR_GREEN, COLOR_RESET, username, client_ip);
            write_log("[LOGIN] user '%s' connected from %s", username, client_ip);
            
            // Create a new thread for the client
            if (pthread_create(&clients[client_idx]->thread, NULL, (void *)handle_client, (void *)(intptr_t)(client_idx)) != 0) 
            {
                write_log("ERROR: Failed to create thread for client %s", username);
                perror("Failed to create thread");
                remove_client(client_idx);
                continue;
            }
            
            // Detach the thread
            pthread_detach(clients[client_idx]->thread);
        }
    }
    
    // Cleanup and exit
    close(server_socket);
    close(signal_pipe[0]);
    close(signal_pipe[1]);
    cleanup_and_exit();
    
    return 0;
}

void setup_server_directories() 
{
    // Create uploads directory
    if (mkdir("uploads", 0777) == -1) 
    {
        if (errno != EEXIST) 
        {
            perror("Failed to create uploads directory");
        }
    }
    
    // Create received_files directory for clients
    if (mkdir("received_files", 0777) == -1) 
    {
        if (errno != EEXIST) 
        {
            perror("Failed to create received_files directory");
        }
    }
}

void write_log(const char *format, ...) 
{
    if (!log_file) return;
    
    pthread_mutex_lock(&log_mutex);
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(log_file, "%04d-%02d-%02d %02d:%02d:%02d - ", 
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, 
            t->tm_hour, t->tm_min, t->tm_sec);
    
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
    
    fprintf(log_file, "\n");
    fflush(log_file);
    
    pthread_mutex_unlock(&log_mutex);
}

void send_message(int client_socket, const char *message) 
{
    send(client_socket, message, strlen(message), 0);
}

void handle_client(void *arg) 
{
    int client_idx = (intptr_t)arg;
    int client_socket;
    char buffer[MAX_MSG_SIZE];
    ssize_t bytes_received;
    
    pthread_mutex_lock(&clients_mutex);
    if (clients[client_idx] == NULL) 
    {
        pthread_mutex_unlock(&clients_mutex);
        return;
    }
    client_socket = clients[client_idx]->socket;
    pthread_mutex_unlock(&clients_mutex);
    
    // Client handling loop
    while (keep_running) 
    {
        memset(buffer, 0, sizeof(buffer));
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received <= 0) {
            // Client disconnected
            pthread_mutex_lock(&clients_mutex);
            if (clients[client_idx] != NULL) 
            {
                printf("%s[DISCONNECT]%s User '%s' lost connection\n", COLOR_RED, COLOR_RESET, clients[client_idx]->username);
                write_log("[DISCONNECT] user '%s' lost connection. Cleaned up resources.", 
                          clients[client_idx]->username);
                clients[client_idx]->connected = false; // Mark as disconnected
                leave_room(client_idx);
                remove_client(client_idx);
            }
            pthread_mutex_unlock(&clients_mutex);
            break;
        }
        
        buffer[bytes_received] = '\0';
        
        // Check if client is in file transfer mode
        pthread_mutex_lock(&clients_mutex);
        bool is_file_transfer = false;
        if (clients[client_idx] != NULL) {
            pthread_mutex_lock(&clients[client_idx]->client_mutex);
            is_file_transfer = clients[client_idx]->file_transfer_in_progress;
            pthread_mutex_unlock(&clients[client_idx]->client_mutex);
        }
        pthread_mutex_unlock(&clients_mutex);
        
        // If not in file transfer mode, process as command
        if (!is_file_transfer) {
            process_command(client_idx, buffer);
        }
        // If in file transfer mode, the file transfer worker will handle the data
    }
    
    return;
}

void process_command(int client_idx, char *command) 
{
    char cmd[MAX_CMD_SIZE], args[MAX_MSG_SIZE];
    char response[MAX_MSG_SIZE];
    
    // Validate command contains only printable characters (filter out file data)
    for (int i = 0; command[i] != '\0'; i++) {
        if (!isprint(command[i]) && command[i] != '\n' && command[i] != '\r') {
            // Contains non-printable characters, likely file data - ignore
            write_log("[WARNING] Ignoring command with non-printable characters from client %d", client_idx);
            return;
        }
    }
    
    // Extract command and arguments
    char *space = strchr(command, ' ');
    if (space) {
        size_t cmd_len = space - command;
        strncpy(cmd, command, cmd_len);
        cmd[cmd_len] = '\0';
        strcpy(args, space + 1);
    } else {
        strcpy(cmd, command);
        args[0] = '\0';
    }
    
    pthread_mutex_lock(&clients_mutex);
    if (clients[client_idx] == NULL) {
        pthread_mutex_unlock(&clients_mutex);
        return;
    }
    
    client_t *client = clients[client_idx];
    pthread_mutex_unlock(&clients_mutex);
    
    // Process commands
    if (strcmp(cmd, "/join") == 0) {
        // Join a room
        if (strlen(args) == 0) {
            sprintf(response, "%sERROR: Missing room name%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        if (!is_valid_roomname(args)) {
            sprintf(response, "%sERROR: Invalid room name. Use only alphanumeric characters (max 32)%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        // Truncate room name if too long to prevent buffer issues
        char safe_room_name[33];  // 32 chars + null terminator
        strncpy(safe_room_name, args, 32);
        safe_room_name[32] = '\0';
        
        // Check if user is switching rooms
        char old_room[MAX_ROOMNAME_LEN] = "";
        bool is_room_switch = false;
        
        if (strlen(client->current_room) > 0) {
            strcpy(old_room, client->current_room);
            is_room_switch = true;
            leave_room(client_idx);
        }
        
        int result = create_or_join_room(client_idx, safe_room_name);
        if (result == 0) {
            // Check if this is a rejoin or first-time join
            bool is_rejoin = has_been_in_room(client_idx, safe_room_name);
            
            // Send appropriate success message based on rejoin status
            if (is_rejoin) {
                snprintf(response, sizeof(response), "%sSUCCESS: Rejoined room '%s' (previous messages not shown)%s", COLOR_GREEN, safe_room_name, COLOR_RESET);
            } else {
                snprintf(response, sizeof(response), "%sSUCCESS: Joined room '%s'%s", COLOR_GREEN, safe_room_name, COLOR_RESET);
            }
            send_message(client->socket, response);
            
            // Add room to user's history if not already there
            if (!is_rejoin) {
                add_room_to_history(client_idx, safe_room_name);
            }
            
            // Log room switch, rejoin, or regular join
            if (is_room_switch && is_rejoin) {
                printf("%s[ROOM]%s User '%s' left '%s', rejoined '%s'\n", COLOR_CYAN, COLOR_RESET, client->username, old_room, safe_room_name);
                write_log("[ROOM] user '%s' left room '%s', rejoined '%s'", client->username, old_room, safe_room_name);
            } else if (is_room_switch && !is_rejoin) {
                printf("%s[ROOM]%s User '%s' left '%s', joined '%s'\n", COLOR_CYAN, COLOR_RESET, client->username, old_room, safe_room_name);
                write_log("[ROOM] user '%s' left room '%s', joined '%s'", client->username, old_room, safe_room_name);
            } else if (!is_room_switch && is_rejoin) {
                printf("%s[ROOM]%s User '%s' rejoined '%s'\n", COLOR_CYAN, COLOR_RESET, client->username, safe_room_name);
                write_log("[ROOM] user '%s' rejoined '%s'", client->username, safe_room_name);
            } else {
                printf("%s[JOIN]%s User '%s' joined room '%s'\n", COLOR_CYAN, COLOR_RESET, client->username, safe_room_name);
                write_log("[JOIN] user '%s' joined room '%s'", client->username, safe_room_name);
            }
        } else if (result == -1) {
            snprintf(response, sizeof(response), "%sERROR: Room '%s' is full%s", COLOR_RED, safe_room_name, COLOR_RESET);
            send_message(client->socket, response);
            
            // If room switch failed, rejoin the old room
            if (is_room_switch) {
                create_or_join_room(client_idx, old_room);
                write_log("[ROOM] user '%s' failed to switch from '%s' to '%s' (room full), stayed in '%s'", 
                         client->username, old_room, safe_room_name, old_room);
            }
        } else {
            sprintf(response, "%sERROR: Failed to join room%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            
            // If room switch failed, rejoin the old room
            if (is_room_switch) {
                create_or_join_room(client_idx, old_room);
                write_log("[ROOM] user '%s' failed to switch from '%s' to '%s' (join failed), stayed in '%s'", 
                         client->username, old_room, safe_room_name, old_room);
            }
        }
    }
    else if (strcmp(cmd, "/leave") == 0) {
        // Leave current room
        if (strlen(client->current_room) == 0) {
            sprintf(response, "%sERROR: You are not in any room%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        char old_room[MAX_ROOMNAME_LEN];
        strcpy(old_room, client->current_room);
        leave_room(client_idx);
        
        snprintf(response, sizeof(response), "%sSUCCESS: Left room '%s'%s", COLOR_GREEN, old_room, COLOR_RESET);
        send_message(client->socket, response);
        printf("%s[LEAVE]%s User '%s' left room '%s'\n", COLOR_YELLOW, COLOR_RESET, client->username, old_room);
        write_log("[ROOM] user '%s' left room '%s'", client->username, old_room);
    }
    else if (strcmp(cmd, "/broadcast") == 0) {
        // Broadcast message to room
        if (strlen(client->current_room) == 0) {
            sprintf(response, "%sERROR: You are not in any room%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        if (strlen(args) == 0) {
            sprintf(response, "%sERROR: Missing message%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        broadcast_message(client_idx, args);
        printf("%s[BROADCAST]%s User '%s' in room '%s': %s\n", COLOR_MAGENTA, COLOR_RESET, client->username, client->current_room, args);
        write_log("[BROADCAST] user '%s': %s", client->username, args);
    }
    else if (strcmp(cmd, "/whisper") == 0) {
        // Send private message
        char recipient[MAX_USERNAME_LEN], message[MAX_MSG_SIZE];
        
        // Parse recipient and message
        char *msg_start = strchr(args, ' ');
        if (!msg_start) {
            sprintf(response, "%sERROR: Invalid whisper format. Use /whisper <username> <message>%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        size_t recipient_len = msg_start - args;
        strncpy(recipient, args, recipient_len);
        recipient[recipient_len] = '\0';
        strcpy(message, msg_start + 1);
        
        if (strlen(message) == 0) {
            sprintf(response, "%sERROR: Missing message%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        whisper_message(client_idx, recipient, message);
        printf("%s[WHISPER]%s From '%s' to '%s': %s\n", COLOR_BLUE, COLOR_RESET, client->username, recipient, message);
        write_log("[WHISPER] From '%s' to '%s': %s", client->username, recipient, message);
    }
    else if (strcmp(cmd, "/sendfile") == 0) {
        // Send file to user
        char filename[256], recipient[MAX_USERNAME_LEN];
        
        // Parse filename and recipient
        char *recipient_start = strchr(args, ' ');
        if (!recipient_start) {
            sprintf(response, "%sERROR: Invalid format. Use /sendfile <filename> <username>%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        size_t filename_len = recipient_start - args;
        strncpy(filename, args, filename_len);
        filename[filename_len] = '\0';
        strcpy(recipient, recipient_start + 1);
        
        // Validate file extension
        if (!is_valid_file_extension(filename)) {
            sprintf(response, "%sERROR: Invalid file type. Accepted: .txt, .pdf, .jpg, .png%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        // Find recipient
        int recipient_idx = find_client_by_username(recipient);
        if (recipient_idx == -1) {
            snprintf(response, sizeof(response), "%sERROR: User '%s' not found%s", COLOR_RED, recipient, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        // Request file size from client
        snprintf(response, sizeof(response), "REQUEST_FILE_SIZE:%s", filename);
        send_message(client->socket, response);
        
        // Wait for file size response
        memset(response, 0, sizeof(response));
        ssize_t bytes_received = recv(client->socket, response, sizeof(response) - 1, 0);
        if (bytes_received <= 0) {
            return;
        }
        response[bytes_received] = '\0';
        
        // Parse file size
        if (strncmp(response, "FILE_SIZE:", 10) != 0) {
            sprintf(response, "%sERROR: Invalid file size response%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            return;
        }
        
        size_t file_size = strtoull(response + 10, NULL, 10);
        if (file_size > MAX_FILE_SIZE) {
            sprintf(response, "%sERROR: File size exceeds limit (3MB)%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            write_log("[ERROR] File '%s' from user '%s' exceeds size limit.", filename, client->username);
            return;
        }
        
        // Add to upload queue
        int queue_result = add_to_upload_queue(filename, client->username, recipient, client->socket, file_size);
        if (queue_result == 0) {
            // Check how many transfers are ahead in queue
            int queue_position = get_queue_position(client->username, filename);
            if (queue_position > 1) {
                snprintf(response, sizeof(response), "%sSUCCESS: File '%s' queued for upload to '%s' (position %d in queue)%s", 
                         COLOR_GREEN, filename, recipient, queue_position, COLOR_RESET);
            } else {
                snprintf(response, sizeof(response), "%sSUCCESS: File '%s' queued for upload to '%s'%s", 
                         COLOR_GREEN, filename, recipient, COLOR_RESET);
            }
            send_message(client->socket, response);
            printf("%s[FILE-QUEUE]%s User '%s' sending '%s' to '%s' (position %d)%s\n", 
                   COLOR_YELLOW, COLOR_RESET, client->username, filename, recipient, queue_position, COLOR_RESET);
            write_log("[FILE-QUEUE] Upload '%s' from %s to %s added to queue at position %d.", 
                      filename, client->username, recipient, queue_position);
        } else if (queue_result == -1) {
            sprintf(response, "%sERROR: Upload queue is full, try again later%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
            write_log("[FILE-QUEUE] Upload queue full - rejected '%s' from %s to %s", filename, client->username, recipient);
        } else {
            sprintf(response, "%sERROR: Failed to queue file upload%s", COLOR_RED, COLOR_RESET);
            send_message(client->socket, response);
        }
    }
    else if (strcmp(cmd, "/exit") == 0) {
        // Disconnect client
        sprintf(response, "%sSUCCESS: Disconnecting from server...%s", COLOR_GREEN, COLOR_RESET);
        send_message(client->socket, response);
        
        pthread_mutex_lock(&clients_mutex);
        if (clients[client_idx] != NULL) {
            printf("%s[LOGOUT]%s User '%s' disconnected\n", COLOR_RED, COLOR_RESET, clients[client_idx]->username);
            write_log("[LOGOUT] user '%s' disconnected. Cleaned up resources.", client->username);
            pthread_mutex_unlock(&clients_mutex);
            leave_room(client_idx);
            remove_client(client_idx);
        }
    }
    else {
        // Unknown command
        sprintf(response, "%sERROR: Unknown command. Available commands: /join, /leave, /broadcast, /whisper, /sendfile, /exit%s", COLOR_RED, COLOR_RESET);
        send_message(client->socket, response);
    }
}

int find_client_by_username(const char *username) 
{
    int client_idx = -1;
    
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (clients[i] != NULL && strcmp(clients[i]->username, username) == 0) 
        {
            client_idx = i;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    
    return client_idx;
}

int add_client(int client_socket, const char *username, const char *ip) 
{
    int idx = -1;
    
    pthread_mutex_lock(&clients_mutex);
    
    // Find empty slot
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (clients[i] == NULL) {
            idx = i;
            break;
        }
    }
    
    // If not found, create new client
    if (idx != -1) 
    {
        clients[idx] = (client_t *)malloc(sizeof(client_t));
        if (clients[idx] != NULL) 
        {
            clients[idx]->socket = client_socket;
            strncpy(clients[idx]->username, username, MAX_USERNAME_LEN - 1);
            clients[idx]->username[MAX_USERNAME_LEN - 1] = '\0';
            strncpy(clients[idx]->ip_address, ip, INET_ADDRSTRLEN - 1);
            clients[idx]->ip_address[INET_ADDRSTRLEN - 1] = '\0';
            clients[idx]->current_room[0] = '\0';
            clients[idx]->connected = true;
            clients[idx]->num_previous_rooms = 0;
            clients[idx]->file_transfer_in_progress = false;
            pthread_mutex_init(&clients[idx]->client_mutex, NULL);
        } else {
            idx = -1;
        }
    }
    
    pthread_mutex_unlock(&clients_mutex);
    return idx;
}

void remove_client(int client_idx) 
{
    pthread_mutex_lock(&clients_mutex);
    
    if (clients[client_idx] != NULL) 
    {
        // Mark as disconnected first
        clients[client_idx]->connected = false;
        
        // Close socket if still open
        if (clients[client_idx]->socket > 0) 
        {
            shutdown(clients[client_idx]->socket, SHUT_RDWR);
            close(clients[client_idx]->socket);
        }
        
        // Destroy the client mutex
        pthread_mutex_destroy(&clients[client_idx]->client_mutex);
        
        // Free memory and clear pointer
        free(clients[client_idx]);
        clients[client_idx] = NULL;
    }
    
    pthread_mutex_unlock(&clients_mutex);
}

int create_or_join_room(int client_idx, const char *room_name) 
{
    int result = -2; // Default error
    
    pthread_mutex_lock(&clients_mutex);
    if (clients[client_idx] == NULL) 
    {
        pthread_mutex_unlock(&clients_mutex);
        return result;
    }
    client_t *client = clients[client_idx];
    pthread_mutex_unlock(&clients_mutex);
    
    pthread_mutex_lock(&rooms_mutex);
    
    // Find the room or create a new one
    int room_idx = -1;
    int free_idx = -1;
    
    for (int i = 0; i < MAX_ROOMS; i++) 
    {
        if (rooms[i].active && strcmp(rooms[i].name, room_name) == 0) 
        {
            room_idx = i;
            break;
        }
        if (!rooms[i].active && free_idx == -1) 
        {
            free_idx = i;
        }
    }
    
    // If room found, try to join
    if (room_idx != -1) 
    {
        pthread_mutex_lock(&rooms[room_idx].room_mutex);
        
        // Check if room is full
        if (rooms[room_idx].num_clients >= MAX_ROOM_CAPACITY) 
        {
            result = -1; // Room full
        } else {
            // Add client to room
            rooms[room_idx].clients[rooms[room_idx].num_clients++] = client;
            strncpy(client->current_room, room_name, MAX_ROOMNAME_LEN - 1);
            client->current_room[MAX_ROOMNAME_LEN - 1] = '\0';
            result = 0; // Success
            
            printf("%s[ROOM-CREATE]%s New room '%s' created by '%s'\n", COLOR_CYAN, COLOR_RESET, room_name, client->username);
            write_log("[ROOM] New room '%s' created by user '%s'", room_name, client->username);
        }
        
        pthread_mutex_unlock(&rooms[room_idx].room_mutex);
    }
    // If room not found, create new room if possible
    else if (free_idx != -1) {
        room_idx = free_idx;
        rooms[room_idx].active = true;
        strncpy(rooms[room_idx].name, room_name, MAX_ROOMNAME_LEN - 1);
        rooms[room_idx].name[MAX_ROOMNAME_LEN - 1] = '\0';
        rooms[room_idx].num_clients = 1;
        rooms[room_idx].clients[0] = client;
        strncpy(client->current_room, room_name, MAX_ROOMNAME_LEN - 1);
        client->current_room[MAX_ROOMNAME_LEN - 1] = '\0';
        result = 0; // Success
        
        printf("%s[ROOM-CREATE]%s New room '%s' created by '%s'\n", COLOR_CYAN, COLOR_RESET, room_name, client->username);
        write_log("[ROOM] New room '%s' created by user '%s'", room_name, client->username);
    }
    
    pthread_mutex_unlock(&rooms_mutex);
    return result;
}

void leave_room(int client_idx) {
    pthread_mutex_lock(&clients_mutex);
    if (clients[client_idx] == NULL || strlen(clients[client_idx]->current_room) == 0) 
    {
        pthread_mutex_unlock(&clients_mutex);
        return;
    }
    
    client_t *client = clients[client_idx];
    char room_name[MAX_ROOMNAME_LEN];
    strncpy(room_name, client->current_room, MAX_ROOMNAME_LEN);
    
    pthread_mutex_unlock(&clients_mutex);
    
    pthread_mutex_lock(&rooms_mutex);
    // Find the room
    for (int i = 0; i < MAX_ROOMS; i++) 
    {
        if (rooms[i].active && strcmp(rooms[i].name, room_name) == 0) 
        {
            pthread_mutex_lock(&rooms[i].room_mutex);
            
            // Find and remove the client from the room
            for (int j = 0; j < rooms[i].num_clients; j++) 
            {
                if (rooms[i].clients[j] == client) 
                {
                    // Shift remaining clients
                    for (int k = j; k < rooms[i].num_clients - 1; k++) 
                    {
                        rooms[i].clients[k] = rooms[i].clients[k + 1];
                    }
                    rooms[i].num_clients--;
                    break;
                }
            }
            
            // If room is empty, mark it as inactive
            if (rooms[i].num_clients == 0) 
            {
                rooms[i].active = false;
                write_log("[ROOM] Room '%s' is now empty and marked inactive", room_name);
            }
            
            pthread_mutex_unlock(&rooms[i].room_mutex);
            break;
        }
    }
    
    pthread_mutex_unlock(&rooms_mutex);
    
    // Clear the client's current room
    pthread_mutex_lock(&clients_mutex);
    if (clients[client_idx] != NULL) 
    {
        clients[client_idx]->current_room[0] = '\0';
    }
    pthread_mutex_unlock(&clients_mutex);
}

void broadcast_message(int client_idx, const char *message) 
{
    char formatted_msg[MAX_MSG_SIZE];
    
    pthread_mutex_lock(&clients_mutex);
    
    if (clients[client_idx] == NULL || strlen(clients[client_idx]->current_room) == 0) 
    {
        pthread_mutex_unlock(&clients_mutex);
        return;
    }
    
    client_t *sender = clients[client_idx];
    char room_name[MAX_ROOMNAME_LEN];
    strncpy(room_name, sender->current_room, MAX_ROOMNAME_LEN);
    
    pthread_mutex_unlock(&clients_mutex);
    
    sprintf(formatted_msg, "%s[ROOM '%s'] %s: %s%s", COLOR_CYAN, room_name, sender->username, message, COLOR_RESET);
    
    pthread_mutex_lock(&rooms_mutex);
    
    // Find the room
    for (int i = 0; i < MAX_ROOMS; i++) 
    {
        if (rooms[i].active && strcmp(rooms[i].name, room_name) == 0) 
        {
            pthread_mutex_lock(&rooms[i].room_mutex);
            
            // Send message to all clients in the room
            for (int j = 0; j < rooms[i].num_clients; j++) 
            {
                if (rooms[i].clients[j] != sender) 
                {
                    send_message(rooms[i].clients[j]->socket, formatted_msg);
                }
            }
            
            // Send confirmation to sender
            sprintf(formatted_msg, "%sSUCCESS: Message broadcast to room '%s'%s", COLOR_GREEN, room_name, COLOR_RESET);
            send_message(sender->socket, formatted_msg);
            
            pthread_mutex_unlock(&rooms[i].room_mutex);
            break;
        }
    }
    
    pthread_mutex_unlock(&rooms_mutex);
}

void whisper_message(int client_idx, const char *recipient_name, const char *message) {
    char formatted_msg[MAX_MSG_SIZE];
    
    pthread_mutex_lock(&clients_mutex);
    
    if (clients[client_idx] == NULL) {
        pthread_mutex_unlock(&clients_mutex);
        return;
    }
    
    client_t *sender = clients[client_idx];
    
    // Find recipient
    client_t *recipient = NULL;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL && strcmp(clients[i]->username, recipient_name) == 0) {
            recipient = clients[i];
            break;
        }
    }
    
    if (recipient == NULL) {
        // Recipient not found
        snprintf(formatted_msg, sizeof(formatted_msg), "%sERROR: User '%s' not found%s", COLOR_RED, recipient_name, COLOR_RESET);
        send_message(sender->socket, formatted_msg);
        pthread_mutex_unlock(&clients_mutex);
        return;
    }
    
    // Send the message to recipient
    sprintf(formatted_msg, "%s[WHISPER] from %s: %s%s", COLOR_MAGENTA, sender->username, message, COLOR_RESET);
    send_message(recipient->socket, formatted_msg);
    
    // Send confirmation to sender
    sprintf(formatted_msg, "%sSUCCESS: Message sent to %s%s", COLOR_GREEN, recipient_name, COLOR_RESET);
    send_message(sender->socket, formatted_msg);
    
    pthread_mutex_unlock(&clients_mutex);
}

int add_to_upload_queue(const char *filename, const char *sender, const char *recipient, int sender_socket, size_t file_size) {
    int result = -2; // Default error
    
    // Try to acquire a slot from the semaphore (non-blocking)
    if (sem_trywait(&upload_semaphore) != 0) {
        return -1; // Queue full
    }
    
    pthread_mutex_lock(&queue_mutex);
    
    // Find empty slot in queue
    for (int i = 0; i < MAX_UPLOAD_QUEUE; i++) {
        if (!upload_queue[i].in_progress) {
            // Add to queue
            strncpy(upload_queue[i].filename, filename, sizeof(upload_queue[i].filename) - 1);
            upload_queue[i].filename[sizeof(upload_queue[i].filename) - 1] = '\0';
            
            strncpy(upload_queue[i].sender, sender, MAX_USERNAME_LEN - 1);
            upload_queue[i].sender[MAX_USERNAME_LEN - 1] = '\0';
            
            strncpy(upload_queue[i].recipient, recipient, MAX_USERNAME_LEN - 1);
            upload_queue[i].recipient[MAX_USERNAME_LEN - 1] = '\0';
            
            upload_queue[i].sender_socket = sender_socket;
            upload_queue[i].request_time = time(NULL);
            upload_queue[i].start_time = 0;  // Will be set when transfer actually starts
            upload_queue[i].in_progress = true;
            upload_queue[i].file_size = file_size;
            
            result = 0; // Success
            
            // Signal the file transfer worker that work is available
            pthread_cond_signal(&queue_cond);
            break;
        }
    }
    
    pthread_mutex_unlock(&queue_mutex);
    
    if (result != 0) {
        // Release the semaphore if we didn't use it
        sem_post(&upload_semaphore);
    }
    
    return result;
}

int get_queue_position(const char *sender, const char *filename) {
    int position = 0;
    
    pthread_mutex_lock(&queue_mutex);
    
    for (int i = 0; i < MAX_UPLOAD_QUEUE; i++) 
    {
        if (upload_queue[i].in_progress) 
        {
            position++;
            if (strcmp(upload_queue[i].sender, sender) == 0 && 
                strcmp(upload_queue[i].filename, filename) == 0) 
                {
                break;
            }
        }
    }
    
    pthread_mutex_unlock(&queue_mutex);
    return position;
}

void* file_transfer_worker(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    char buffer[FILE_TRANSFER_BUFFER];
    char response[MAX_MSG_SIZE];
    char filepath[1024];  // Increased size to handle long paths
    char recipient_path[1024];  // Increased size to handle long paths
    FILE *file;
    int recipient_idx;
    
    // Signal that the worker thread is ready
    pthread_mutex_lock(&worker_ready_mutex);
    worker_thread_ready = true;
    pthread_cond_signal(&worker_ready_cond);
    pthread_mutex_unlock(&worker_ready_mutex);
    
    while (keep_running) 
    {
        pthread_mutex_lock(&queue_mutex);
        
        // Find next file transfer in queue
        file_transfer_t *transfer = NULL;
        int transfer_idx = -1;
        
        for (int i = 0; i < MAX_UPLOAD_QUEUE; i++) 
        {
            if (upload_queue[i].in_progress) 
            {
                transfer = &upload_queue[i];
                transfer_idx = i;
                break;
            }
        }
        
        if (transfer != NULL) {
            pthread_mutex_unlock(&queue_mutex);
            
            // Declare sender_idx for use throughout transfer
            int sender_idx = -1;
            
            // Process file transfer
            recipient_idx = find_client_by_username(transfer->recipient);
            if (recipient_idx == -1) 
            {
                // Recipient not found
                snprintf(response, sizeof(response), "%sERROR: User '%s' not found%s", COLOR_RED, transfer->recipient, COLOR_RESET);
                send_message(transfer->sender_socket, response);
                
                write_log("[FILE] Failed: '%s' from '%s' to '%s'. Recipient not found.", 
                          transfer->filename, transfer->sender, transfer->recipient);
            } else {
                // Create unique filename to prevent collisions
                char unique_filename[512];  // Increased size to accommodate timestamp + filename
                time_t now = time(NULL);
                snprintf(unique_filename, sizeof(unique_filename), "%lu_%s", (unsigned long)now, transfer->filename);
                
                // Create upload directory
                snprintf(filepath, sizeof(filepath), "uploads/%s", unique_filename);
                
                // Create recipient directory
                char recipient_dir[256];
                sprintf(recipient_dir, "received_files/%s", transfer->recipient);
                if (mkdir(recipient_dir, 0777) == -1) 
                {
                    if (errno != EEXIST) {
                        write_log("[ERROR] Failed to create directory for recipient: %s", transfer->recipient);
                    }
                }
                
                // Check for filename conflicts and generate unique filename if needed
                char* final_filename = generate_unique_filename(recipient_dir, transfer->filename);
                bool conflict_detected = (strcmp(final_filename, transfer->filename) != 0);
                
                snprintf(recipient_path, sizeof(recipient_path), "%s/%s", recipient_dir, final_filename);
                
                // Log filename conflict if detected
                if (conflict_detected) 
                {
                    printf("%s[FILE-CONFLICT]%s '%s' → renamed to '%s'\n", COLOR_YELLOW, COLOR_RESET, transfer->filename, final_filename);
                    write_log("[FILE] Conflict: '%s' received twice → renamed '%s'", 
                              transfer->filename, final_filename);
                }
                
                // Record when transfer actually starts and calculate wait time
                transfer->start_time = time(NULL);
                double wait_time = difftime(transfer->start_time, transfer->request_time);
                
                // Log wait time if file was queued
                if (wait_time > 1.0) 
                {
                    printf("%s[FILE-QUEUE]%s '%s' from '%s' started upload after %.0f seconds in queue\n", 
                           COLOR_YELLOW, COLOR_RESET, transfer->filename, transfer->sender, wait_time);
                    write_log("[FILE] '%s' from user '%s' started upload after %.0f seconds in queue.", 
                              transfer->filename, transfer->sender, wait_time);
                }
                
                // Tell sender to start sending file
                sprintf(response, "START_UPLOAD:%s", transfer->filename);
                send_message(transfer->sender_socket, response);
                
                // Set client to file transfer mode
                pthread_mutex_lock(&clients_mutex);
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i] != NULL && clients[i]->socket == transfer->sender_socket) {
                        sender_idx = i;
                        pthread_mutex_lock(&clients[i]->client_mutex);
                        clients[i]->file_transfer_in_progress = true;
                        pthread_mutex_unlock(&clients[i]->client_mutex);
                        break;
                    }
                }
                pthread_mutex_unlock(&clients_mutex);
                
                // Receive and write file
                file = fopen(filepath, "wb");
                if (file) 
                {
                    size_t total_received = 0;
                    time_t start_time = time(NULL);
                    write_log("[FILE] Started upload '%s' from '%s' to '%s'. Size: %lu bytes", 
                              transfer->filename, transfer->sender, transfer->recipient, 
                              (unsigned long)transfer->file_size);
                    
                    while (total_received < transfer->file_size) {
                        memset(buffer, 0, sizeof(buffer));
                        
                        // Calculate how many bytes to read (don't exceed file size)
                        size_t bytes_to_read = sizeof(buffer);
                        if (total_received + bytes_to_read > transfer->file_size) {
                            bytes_to_read = transfer->file_size - total_received;
                        }
                        
                        ssize_t bytes = recv(transfer->sender_socket, buffer, bytes_to_read, 0);
                        
                        if (bytes <= 0) {
                            write_log("[FILE] Error: Connection lost during transfer of '%s' from '%s'", 
                                      transfer->filename, transfer->sender);
                            break;
                        }
                        
                        fwrite(buffer, 1, bytes, file);
                        total_received += bytes;
                        
                        // Safety check - should never happen but prevents overflow
                        if (total_received > transfer->file_size) {
                            write_log("[FILE] Error: Received more data than expected for '%s' from '%s' (%lu > %lu)", 
                                      transfer->filename, transfer->sender, 
                                      (unsigned long)total_received, (unsigned long)transfer->file_size);
                            break;
                        }
                    }
                    
                    fclose(file);
                    time_t end_time = time(NULL);
                    double duration = difftime(end_time, start_time);

                    if (total_received == transfer->file_size) 
                    {
                        // Copy file to recipient's directory
                        FILE *src = fopen(filepath, "rb");
                        FILE *dest = fopen(recipient_path, "wb");
                        
                        if (src && dest) 
                        {
                            while (1) {
                                size_t bytes = fread(buffer, 1, sizeof(buffer), src);
                                if (bytes == 0) break;
                                fwrite(buffer, 1, bytes, dest);
                            }
                            
                            fclose(src);
                            fclose(dest);
                            
                            // Notify sender with final filename used
                            if (conflict_detected) {
                                sprintf(response, "%sSUCCESS: File '%s' sent to '%s' (saved as '%s' due to name conflict)%s", 
                                        COLOR_GREEN, transfer->filename, transfer->recipient, final_filename, COLOR_RESET);
                            } else {
                                sprintf(response, "%sSUCCESS: File '%s' sent to '%s'%s", 
                                        COLOR_GREEN, transfer->filename, transfer->recipient, COLOR_RESET);
                            }
                            send_message(transfer->sender_socket, response);
                            
                            // Notify recipient with final filename
                            pthread_mutex_lock(&clients_mutex);
                            if (clients[recipient_idx] != NULL) 
                            {
                                if (conflict_detected) 
                                {
                                    sprintf(response, "%s[FILE] Received file '%s' from '%s' (saved as '%s')%s", 
                                            COLOR_BLUE, transfer->filename, transfer->sender, final_filename, COLOR_RESET);
                                } else {
                                    sprintf(response, "%s[FILE] Received file '%s' from '%s'%s", 
                                            COLOR_BLUE, transfer->filename, transfer->sender, COLOR_RESET);
                                }
                                send_message(clients[recipient_idx]->socket, response);
                            }
                            pthread_mutex_unlock(&clients_mutex);
                            
                            // Log success with final filename
                            if (conflict_detected) 
                            {
                                printf("%s[FILE-SUCCESS]%s '%s' from '%s' to '%s' (renamed to '%s')\n", 
                                       COLOR_GREEN, COLOR_RESET, transfer->filename, transfer->sender, transfer->recipient, final_filename);
                                write_log("[FILE] Success: '%s' from '%s' to '%s' completed in %.1f seconds (renamed to '%s')", 
                                          transfer->filename, transfer->sender, transfer->recipient, duration, final_filename);
                            } else {
                                printf("%s[FILE-SUCCESS]%s '%s' from '%s' to '%s'\n", 
                                       COLOR_GREEN, COLOR_RESET, transfer->filename, transfer->sender, transfer->recipient);
                                write_log("[FILE] Success: '%s' from '%s' to '%s' completed in %.1f seconds", 
                                          transfer->filename, transfer->sender, transfer->recipient, duration);
                            }
                        } else {
                            // File copy error
                            if (src) fclose(src);
                            if (dest) fclose(dest);
                            
                            sprintf(response, "%sERROR: File transfer completed but delivery failed%s", 
                                    COLOR_RED, COLOR_RESET);
                            send_message(transfer->sender_socket, response);
                            
                            write_log("[FILE] Error: '%s' from '%s' to '%s' - File copy failed", 
                                      transfer->filename, transfer->sender, transfer->recipient);
                        }
                    } else {
                        // Incomplete transfer
                        sprintf(response, "%sERROR: File transfer incomplete. Only received %lu of %lu bytes%s", 
                                COLOR_RED, (unsigned long)total_received, (unsigned long)transfer->file_size, COLOR_RESET);
                        send_message(transfer->sender_socket, response);
                        
                        write_log("[FILE] Error: '%s' from '%s' to '%s' - Incomplete transfer (%lu/%lu bytes)", 
                                  transfer->filename, transfer->sender, transfer->recipient, 
                                  (unsigned long)total_received, (unsigned long)transfer->file_size);
                    }
                } else {
                    // Failed to open file
                    sprintf(response, "%sERROR: Server failed to process file%s", COLOR_RED, COLOR_RESET);
                    send_message(transfer->sender_socket, response);
                    
                    write_log("[FILE] Error: '%s' from '%s' to '%s' - Failed to open file", 
                              transfer->filename, transfer->sender, transfer->recipient);
                }
            }
            
            // Mark transfer as complete
            pthread_mutex_lock(&queue_mutex);
            upload_queue[transfer_idx].in_progress = false;
            pthread_mutex_unlock(&queue_mutex);
            
            // Clear file transfer mode for sender
            if (sender_idx != -1) {
                pthread_mutex_lock(&clients_mutex);
                if (clients[sender_idx] != NULL) {
                    pthread_mutex_lock(&clients[sender_idx]->client_mutex);
                    clients[sender_idx]->file_transfer_in_progress = false;
                    pthread_mutex_unlock(&clients[sender_idx]->client_mutex);
                }
                pthread_mutex_unlock(&clients_mutex);
            }
            
            // Release semaphore slot
            sem_post(&upload_semaphore);
            
            // Signal that a transfer slot is available
            pthread_cond_signal(&queue_cond);
        } else {
            // No transfers, wait for notification instead of polling
            if (keep_running) {
                pthread_cond_wait(&queue_cond, &queue_mutex);
            }
            pthread_mutex_unlock(&queue_mutex);
        }
    }
    
    return NULL;
}

bool is_valid_username(const char *username) 
{
    // Check length
    size_t len = strlen(username);
    if (len == 0 || len > 16) {
        return false;
    }
    
    // Check if alphanumeric
    for (size_t i = 0; i < len; i++) 
    {
        if (!isalnum(username[i])) {
            return false;
        }
    }
    
    return true;
}

bool is_valid_roomname(const char *roomname) 
{
    // Check length
    size_t len = strlen(roomname);
    if (len == 0 || len > 32) 
    {
        return false;
    }
    
    // Check if alphanumeric
    for (size_t i = 0; i < len; i++) 
    {
        if (!isalnum(roomname[i])) {
            return false;
        }
    }
    
    return true;
}

bool is_valid_file_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot) {
        return false;
    }
    
    for (int i = 0; i < NUM_VALID_EXTENSIONS; i++) {
        if (strcasecmp(dot, VALID_EXTENSIONS[i]) == 0) {
            return true;
        }
    }
    
    return false;
}

void handle_sigint(int sig) 
{
    (void)sig;  // Suppress unused parameter warning
    
    // Signal-safe way to notify main loop
    char byte = 1;
    ssize_t result = write(signal_pipe[1], &byte, 1);
    (void)result;  // Suppress unused variable warning
}

void cleanup_and_exit() 
{
    char message[MAX_MSG_SIZE];
    sprintf(message, "%s[SERVER] Server is shutting down. Goodbye!%s", COLOR_YELLOW, COLOR_RESET);
    
    // Set keep_running to false to signal all threads to stop
    keep_running = 0;
    
    // Force close the server socket first to stop accepting new connections
    if (server_socket > 0) {
        shutdown(server_socket, SHUT_RDWR);
        close(server_socket);
    }
    
    // Close signal pipe
    if (signal_pipe[0] > 0) close(signal_pipe[0]);
    if (signal_pipe[1] > 0) close(signal_pipe[1]);
    
    // Notify and disconnect all clients (but handle failures gracefully)
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (clients[i] != NULL && clients[i]->connected) {
            // Try to send goodbye message, but don't block if client is unresponsive
            int flags = fcntl(clients[i]->socket, F_GETFL, 0);
            if (flags != -1) {
                fcntl(clients[i]->socket, F_SETFL, flags | O_NONBLOCK);
            }
            
            // Attempt to send message (non-blocking)
            ssize_t sent = send(clients[i]->socket, message, strlen(message), MSG_DONTWAIT | MSG_NOSIGNAL);
            (void)sent; // Suppress unused variable warning
            
            // Socket is non-blocking, message sent immediately or queued
            
            // Close socket and clean up
            shutdown(clients[i]->socket, SHUT_RDWR);
            close(clients[i]->socket);
            
            // Mark as disconnected to avoid double cleanup
            clients[i]->connected = false;
            
            // Don't use pthread_cancel as it's unsafe - thread will exit naturally
            // when socket closes and keep_running becomes 0
            
            free(clients[i]);
            clients[i] = NULL;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    
    // Signal file transfer worker to wake up and exit
    pthread_cond_broadcast(&queue_cond);
    
    // Threads will exit naturally when sockets close and keep_running becomes 0
    
    // Close log file
    if (log_file) {
        fprintf(log_file, "Server shutdown complete\n");
        fclose(log_file);
    }
    
    printf("%s=== SERVER SHUTDOWN COMPLETE ===%s\n", COLOR_GREEN, COLOR_RESET);
    _exit(0);  // Force immediate exit
}

char* generate_unique_filename(const char* directory, const char* filename) {
    static char result_path[2048];  // Increased size
    char test_path[4096];           // Much larger buffer to avoid warnings
    
    // Validate input lengths to prevent buffer overflow
    size_t dir_len = strlen(directory);
    size_t file_len = strlen(filename);
    
    // Check if combined path would be too long
    if (dir_len + file_len + 2 > sizeof(test_path) - 1) {
        // If too long, just return original filename (truncated if needed)
        strncpy(result_path, filename, sizeof(result_path) - 1);
        result_path[sizeof(result_path) - 1] = '\0';
        return result_path;
    }
    
    // First, try the original filename
    snprintf(test_path, sizeof(test_path), "%s/%s", directory, filename);
    
    // If file doesn't exist, use original name
    if (access(test_path, F_OK) != 0) {
        strncpy(result_path, filename, sizeof(result_path) - 1);
        result_path[sizeof(result_path) - 1] = '\0';
        return result_path;
    }
    
    // File exists, need to generate unique name
    char name_only[1024];   // Increased size
    char extension[256];    // Increased size
    const char *dot = strrchr(filename, '.');
    
    if (dot && dot != filename) {
        // Has extension
        size_t name_len = dot - filename;
        if (name_len >= sizeof(name_only)) {
            name_len = sizeof(name_only) - 1;
        }
        strncpy(name_only, filename, name_len);
        name_only[name_len] = '\0';
        strncpy(extension, dot, sizeof(extension) - 1);
        extension[sizeof(extension) - 1] = '\0';
    } else {
        // No extension
        strncpy(name_only, filename, sizeof(name_only) - 1);
        name_only[sizeof(name_only) - 1] = '\0';
        extension[0] = '\0';
    }
    
    // Try incremental suffixes until we find a unique name
    for (int i = 1; i <= 999; i++) {
        if (strlen(extension) > 0) {
            snprintf(result_path, sizeof(result_path), "%s_%d%s", name_only, i, extension);
        } else {
            snprintf(result_path, sizeof(result_path), "%s_%d", name_only, i);
        }
        
        // Check if the combined path would be too long
        if (dir_len + strlen(result_path) + 2 <= sizeof(test_path) - 1) {
            snprintf(test_path, sizeof(test_path), "%s/%s", directory, result_path);
            
            // If this name doesn't exist, use it
            if (access(test_path, F_OK) != 0) {
                return result_path;
            }
        }
    }
    
    // If we somehow reach here (999+ conflicts), use timestamp fallback
    time_t now = time(NULL);
    if (strlen(extension) > 0) {
        snprintf(result_path, sizeof(result_path), "%s_%lu%s", name_only, (unsigned long)now, extension);
    } else {
        snprintf(result_path, sizeof(result_path), "%s_%lu", name_only, (unsigned long)now);
    }
    
    return result_path;
}

bool has_been_in_room(int client_idx, const char* room_name) 
{
    pthread_mutex_lock(&clients_mutex);
    if (clients[client_idx] == NULL) {
        pthread_mutex_unlock(&clients_mutex);
        return false;
    }
    client_t *client = clients[client_idx];
    pthread_mutex_unlock(&clients_mutex);
    
    for (int i = 0; i < client->num_previous_rooms; i++) 
    {
        if (strcmp(client->previous_rooms[i], room_name) == 0) {
            return true;
        }
    }
    return false;
}

void add_room_to_history(int client_idx, const char* room_name) {
    pthread_mutex_lock(&clients_mutex);
    if (clients[client_idx] == NULL) {
        pthread_mutex_unlock(&clients_mutex);
        return;
    }
    client_t *client = clients[client_idx];
    pthread_mutex_unlock(&clients_mutex);
    
    if (client->num_previous_rooms < MAX_ROOMS) {
        strncpy(client->previous_rooms[client->num_previous_rooms], room_name, MAX_ROOMNAME_LEN - 1);
        client->previous_rooms[client->num_previous_rooms][MAX_ROOMNAME_LEN - 1] = '\0';
        client->num_previous_rooms++;
    }
}