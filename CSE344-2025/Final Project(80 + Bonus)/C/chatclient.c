#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <strings.h> // For strcasecmp

#define MAX_MSG_SIZE 4096
#define MAX_CMD_SIZE 256
#define MAX_USERNAME_LEN 17 // 16 chars + null terminator
#define BUFFER_SIZE 1024
#define FILE_BUFFER_SIZE 1024

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

int server_socket;
char username[MAX_USERNAME_LEN];
volatile sig_atomic_t keep_running = 1;
pthread_t receive_thread;
pthread_mutex_t file_transfer_mutex = PTHREAD_MUTEX_INITIALIZER;

void *receive_messages(void *arg);
int send_file(const char *filename, const char *recipient);
void print_help();
void handle_sigint(int sig);
void cleanup_and_exit();
size_t get_file_size(const char *filename);
bool is_valid_username(const char *username);


int main(int argc, char *argv[]) 
{
    if (argc != 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        return 1;
    }

    // Parse server details
    char *server_ip = argv[1];
    int port = atoi(argv[2]);

    // Validate port number
    if (port <= 0 || port > 65535) 
    {
        printf("Error: Port number must be between 1 and 65535\n");
        return 1;
    }

    // Install signal handler
    signal(SIGINT, handle_sigint);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) 
    {
        perror("Failed to create socket");
        return 1;
    }

    // Configure server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) 
    {
        perror("Invalid address or address not supported");
        close(server_socket);
        return 1;
    }

    // Connect to server
    printf("Connecting to server at %s:%d...\n", server_ip, port);
    if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Connection failed");
        close(server_socket);
        return 1;
    }

    printf("Connected to server!\n");

    // Username validation loop - retry until accepted
    bool username_accepted = false;
    
    while (!username_accepted) 
    {
        // Prompt for username
        do 
        {
            printf("Enter your username (max 16 alphanumeric characters): ");
            if (fgets(username, MAX_USERNAME_LEN, stdin) == NULL) {
                printf("Error reading username\n");
                close(server_socket);
                return 1;
            }

            // Remove newline character
            size_t len = strlen(username);
            if (len > 0 && username[len - 1] == '\n') 
            {
                username[len - 1] = '\0';
                len--;
            }

            if (!is_valid_username(username)) 
            {
                printf("%sInvalid username. Use only alphanumeric characters (max 16).%s\n", COLOR_RED, COLOR_RESET);
            }
        } while (!is_valid_username(username));

        // Send username to server
        if (send(server_socket, username, strlen(username), 0) < 0) {
            perror("Failed to send username");
            close(server_socket);
            return 1;
        }

        // Wait for server response
        char response[MAX_MSG_SIZE];
        ssize_t bytes_received = recv(server_socket, response, sizeof(response) - 1, 0);
        if (bytes_received <= 0) 
        {
            printf("Connection closed by server\n");
            close(server_socket);
            return 1;
        }
        response[bytes_received] = '\0';
        printf("%s\n", response);

        // Check server response
        if (strncmp(response, "SUCCESS:", 8) == 0) 
        {
            username_accepted = true;
        } else if (strncmp(response, "ERROR:", 6) == 0) 
        {
            // Server rejected username, but connection stays open - try again
            printf("%sPlease try a different username.%s\n", COLOR_YELLOW, COLOR_RESET);
        } else {
            printf("Unexpected server response. Disconnecting.\n");
        close(server_socket);
        return 1;
        }
    }

    // Create directory for received files
    char received_dir[256];
    sprintf(received_dir, "received_files/%s", username);
    if (mkdir("received_files", 0777) == -1) {
        if (errno != EEXIST) {
            perror("Failed to create received_files directory");
        }
    }
    if (mkdir(received_dir, 0777) == -1) {
        if (errno != EEXIST) {
            perror("Failed to create user's received files directory");
        }
    }

    // Create receive thread
    if (pthread_create(&receive_thread, NULL, receive_messages, NULL) != 0) {
        perror("Failed to create receive thread");
        close(server_socket);
        return 1;
    }
    
    // Detach the thread so it can clean up its own resources
    pthread_detach(receive_thread);

    // Print help message
    print_help();

    // Main loop for user input
    char command[MAX_CMD_SIZE];
    while (keep_running) 
    {
        printf("> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL) {
            if (feof(stdin) || !keep_running) {
                printf("\nExiting...\n");
                break;
            }
            continue;
        }

        // Check if we should exit (signal received)
        if (!keep_running) {
            break;
        }

        // Remove newline character
        size_t cmd_len = strlen(command);
        if (cmd_len > 0 && command[cmd_len - 1] == '\n') {
            command[cmd_len - 1] = '\0';
            cmd_len--;
        }

        // Skip empty commands
        if (cmd_len == 0) {
            continue;
        }

        // Process commands
        if (strcmp(command, "/help") == 0) {
            print_help();
        }
        else if (strcmp(command, "/exit") == 0) {
            // Send exit command to server
            if (send(server_socket, command, strlen(command), 0) < 0) {
                perror("Failed to send exit command");
            }
            
            // Signal thread to stop and wait for it
            keep_running = 0;
            
            // Server will process exit command immediately
            
            break;
        }
        else if (strncmp(command, "/sendfile", 9) == 0) {
            // Parse sendfile command
            char filename[256] = {0};
            char recipient[MAX_USERNAME_LEN] = {0};
            
            // Extract filename and recipient
            if (sscanf(command + 10, "%255s %16s", filename, recipient) != 2) {
                printf("%sERROR: Invalid format. Use /sendfile <filename> <username>%s\n", COLOR_RED, COLOR_RESET);
                continue;
            }
            
            // Try to send the file
            send_file(filename, recipient);
        }
        else {
            // Send command to server
            if (send(server_socket, command, strlen(command), 0) < 0) {
                perror("Failed to send command");
                break;
            }
        }
    }

    // Cleanup and exit
    cleanup_and_exit();
    
    return 0;
}

void *receive_messages(void *arg) 
{
    // Suppress unused parameter warning
    (void)arg;
    
    char buffer[MAX_MSG_SIZE];
    ssize_t bytes_received;
    
    // Keep receiving messages until the connection is closed or an error occurs
    while (keep_running) 
    {
        memset(buffer, 0, sizeof(buffer));
        bytes_received = recv(server_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printf("\n%sConnection closed by server%s\n", COLOR_RED, COLOR_RESET);
            } else {
                perror("Error receiving message");
            }
            keep_running = 0;
            break;
        }
        
        buffer[bytes_received] = '\0';
        
        // Check for special commands from server
        if (strncmp(buffer, "REQUEST_FILE_SIZE:", 18) == 0) 
        {
            char filename[256];
            strcpy(filename, buffer + 18);
            
            // Get file size
            size_t file_size = get_file_size(filename);
            if (file_size == 0) {
                printf("%sERROR: Could not access file '%s'%s\n", COLOR_RED, filename, COLOR_RESET);
                char error_msg[] = "FILE_SIZE:0";
                send(server_socket, error_msg, strlen(error_msg), 0);
                continue;
            }
            
            // Send file size back to server
            char size_msg[256];
            sprintf(size_msg, "FILE_SIZE:%lu", (unsigned long)file_size);
            send(server_socket, size_msg, strlen(size_msg), 0);
        }
        else if (strncmp(buffer, "START_UPLOAD:", 13) == 0) 
        {
            // Lock to serialize file transfers
            pthread_mutex_lock(&file_transfer_mutex);
            
            char filename[256];
            strcpy(filename, buffer + 13);
            
            // Send file content
            FILE *file = fopen(filename, "rb");
            if (!file) {
                printf("%sERROR: Could not open file '%s'%s\n", COLOR_RED, filename, COLOR_RESET);
                pthread_mutex_unlock(&file_transfer_mutex);
                continue;
            }
            
            char file_buffer[FILE_BUFFER_SIZE];
            size_t bytes_read;
            size_t total_sent = 0;
            size_t file_size = get_file_size(filename);
            
            printf("%sUploading file '%s' (%lu bytes)...%s\n", 
                   COLOR_YELLOW, filename, (unsigned long)file_size, COLOR_RESET);
            
            while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), file)) > 0) 
            {
                ssize_t bytes_sent = send(server_socket, file_buffer, bytes_read, 0);
                if (bytes_sent < 0) {
                    perror("Error sending file");
                    break;
                }
                total_sent += bytes_sent;
                
                // Display progress
                float percent = (float)total_sent / file_size * 100;
                printf("\rProgress: %.1f%% (%lu/%lu bytes)", 
                       percent, (unsigned long)total_sent, (unsigned long)file_size);
                fflush(stdout);
            }
            
            fclose(file);
            printf("\n%sFile upload completed!%s\n", COLOR_GREEN, COLOR_RESET);
            
            // Unlock after file transfer is complete
            pthread_mutex_unlock(&file_transfer_mutex);
        }
        else {
            // Regular message from server, print it
            printf("\r%s\n", buffer);
            if(keep_running) {
                printf("> ");
            }
            fflush(stdout);
        }
    }
    
    return NULL;
}

int send_file(const char *filename, const char *recipient) 
{
    // Check if file exists and is readable
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("%sERROR: Could not open file '%s'%s\n", COLOR_RED, filename, COLOR_RESET);
        return -1;
    }
    fclose(file);
    
    // Check file size
    size_t file_size = get_file_size(filename);
    if (file_size == 0) {
        printf("%sERROR: File '%s' is empty or cannot be accessed%s\n", COLOR_RED, filename, COLOR_RESET);
        return -1;
    }
    
    // Check file extension
    const char *dot = strrchr(filename, '.');
    if (!dot) {
        printf("%sERROR: File has no extension. Accepted: .txt, .pdf, .jpg, .png%s\n", COLOR_RED, COLOR_RESET);
        return -1;
    }
    
    // Check if extension is valid
    const char *valid_extensions[] = {".txt", ".pdf", ".jpg", ".png"};
    const int num_valid_extensions = 4;
    
    bool valid_extension = false;
    for (int i = 0; i < num_valid_extensions; i++) 
    {
        if (strcasecmp(dot, valid_extensions[i]) == 0) {
            valid_extension = true;
            break;
        }
    }
    
    if (!valid_extension) {
        printf("%sERROR: File has invalid extension. Accepted: .txt, .pdf, .jpg, .png%s\n", COLOR_RED, COLOR_RESET);
        return -1;
    }
    
    // Find recipient
    char request[MAX_CMD_SIZE];
    sprintf(request, "/sendfile %s %s", filename, recipient);
    if (send(server_socket, request, strlen(request), 0) < 0) 
    {
        perror("Failed to send file request");
        return -1;
    }
    
    return 0;
}

size_t get_file_size(const char *filename) 
{
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size;
    }
    return 0;
}

void print_help() 
{
    printf("\n%s=== Chat Client Help ===%s\n", COLOR_CYAN, COLOR_RESET);
    printf("Available commands:\n");
    printf("  %s/join <room>%s - Join a chat room\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s/leave%s - Leave the current room\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s/broadcast <message>%s - Send a message to all users in the room\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s/whisper <user> <message>%s - Send a private message to a user\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s/sendfile <filename> <user>%s - Send a file to a user\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s/exit%s - Disconnect from the server\n", COLOR_GREEN, COLOR_RESET);
    printf("  %s/help%s - Show all commands\n", COLOR_GREEN, COLOR_RESET);
    printf("\n");
}

void handle_sigint(int sig) 
{
    (void)sig;
    printf("\n%sReceived Ctrl+C, exiting gracefully...%s\n", COLOR_YELLOW, COLOR_RESET);
    
    // Set flag to stop main loop
    keep_running = 0;
    
    // Try to send exit command to server to notify it properly
    if (server_socket > 0) {
        const char *exit_cmd = "/exit";
        ssize_t sent = send(server_socket, exit_cmd, strlen(exit_cmd), MSG_DONTWAIT | MSG_NOSIGNAL);
        (void)sent; // Suppress unused variable warning
        
        // Server will process exit command immediately or connection is lost
    }
    
    // Cleanup and exit
    cleanup_and_exit();
}

void cleanup_and_exit() 
{
    // Set the global flag to stop all threads
    keep_running = 0;
    
    // Close socket if still connected (this will cause receive thread to exit)
    if (server_socket > 0) {
        shutdown(server_socket, SHUT_RDWR);
        close(server_socket);
        server_socket = -1;
    }
    
    // Since the receive thread is detached, it will clean up automatically
    // when it detects the closed socket and keep_running = 0
    // Thread will exit cleanly when it detects closed socket
    
    printf("%sDisconnected from server%s\n", COLOR_YELLOW, COLOR_RESET);
    exit(0);
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