#include "bankClient.h"

char server_fifo_name[MAX_PATH_LENGTH];
ClientInfo clients[MAX_CLIENTS];
int client_count = 0;
int running = 1;
int client_fifo_fd = -1;

int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <client_file> <server_fifo_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    initialize_client(argv[2]);
    printf("Client is active...\n");

    //Read client file
    if(!read_client_file(argv[1])){
        fprintf(stderr, "Error reading client file\n");
        cleanup_client();
        exit(EXIT_FAILURE);
    }

    printf("Reading %s...\n", argv[1]);
    printf("%d clients to connect...\n", client_count);
    
    //Create client processes
    printf("Creating client processes...\n");
    create_client_processes();

    sleep(3); // Give some time for processes to start

    //Connect to server FIFO
    if(!connect_to_server()){
        fprintf(stderr, "Error connecting to %s\n", server_fifo_name);
        printf("exiting..\n");
        cleanup_client();
        exit(EXIT_FAILURE);
    }

    printf("Connected to %s...\n", server_fifo_name);

    //Wait for all client processes to finish
    while(wait(NULL) > 0){
        //Wait for all child processes to terminate
    }

    cleanup_client();
    printf("exiting..\n");

    return 0;
}

void handle_signals(int signo){
    if(signo == SIGINT || signo == SIGTERM){
        printf("Received signal %d, cleaning up...\n", signo);
        running = 0;

        //Terminate all client processes
        for(int i = 0; i < client_count; i++){
            if(clients[i].connected){
                kill(clients[i].client_num, SIGTERM);
            }
        }
        //Wait for all client processes to terminate
        while(wait(NULL) > 0){
            //Wait for all child processes to terminate
        }

        //Cleanup
        cleanup_client();
        printf("Client processes terminated.\n");
        printf("Bye...\n");

        exit(EXIT_SUCCESS);
    }
}

void initialize_client(const char* server_fifo){
    strncpy(server_fifo_name, server_fifo, MAX_PATH_LENGTH - 1);
    server_fifo_name[MAX_PATH_LENGTH - 1] = '\0'; // Ensure null termination

    //Initialize clients array
    memset(clients, 0, sizeof(clients));
    client_count = 0;

    //Set up signal handlers
    signal(SIGINT, handle_signals);
    signal(SIGTERM, handle_signals);
}

int read_client_file(const char* filename){
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        perror("Error opening client file");
        return 0;
    }

    char line[MAX_BUFFER_SIZE];
    client_count = 0;
    while(fgets(line, sizeof(line), file) != NULL && client_count < MAX_CLIENTS){
        //Remove trailing newline if present
        size_t len = strlen(line);
        if(len > 0 && line[len - 1] == '\n'){
            line[len - 1] = '\0';
        }

        //Parse client transaction
        char account_id[20];
        char operation_str[20];
        int amount;

        int result = sscanf(line, "%s %s %d", account_id, operation_str, &amount);
        if(result == 3){
            ClientInfo* client = &clients[client_count++];
            client->client_num = client_count;
            strncpy(client->account_id, account_id, sizeof(client->account_id) - 1);
            client->account_id[sizeof(client->account_id) - 1] = '\0'; // Ensure null termination
            strncpy(client->transaction.account_id, client->account_id, sizeof(client->transaction.account_id) - 1);
            client->transaction.account_id[sizeof(client->transaction.account_id) - 1] = '\0'; // Ensure null termination

            //Set transaction details
            if(strcmp(operation_str, "deposit") == 0){
                client->transaction.operation = DEPOSIT;
            } else if(strcmp(operation_str, "withdraw") == 0){
                client->transaction.operation = WITHDRAW;
            } else {
                fprintf(stderr, "Invalid operation: %s\n", operation_str);
                client_count--;
                continue;
            }
            client->transaction.amount = amount;
            client->connected = 0; // Not connected yet
        }
    }

    fclose(file);
    return client_count > 0;
}

int connect_to_server(){
    //Open server FIFO for writing
    int server_fd = open(server_fifo_name, O_WRONLY | O_NONBLOCK);
    if(server_fd == -1){
        perror("Error opening server FIFO");
        return 0;
    }

    //Send connection request to server
    struct{
        pid_t client_pid;
        int client_count;
        char operation[MAX_CLIENTS];
    } client_req;

    client_req.client_pid = getpid();
    client_req.client_count = client_count;
    for(int i = 0; i < client_count; i++){
        client_req.operation[i] = clients[i].transaction.operation;
    }

    ssize_t bytes_written = write(server_fd, &client_req, sizeof(client_req));
    close(server_fd);
    
    if(bytes_written == -1){
        perror("Error writing to server FIFO");
        return 0;
    }

    return 1;
}

void handle_signals_child(int signo){
    if(signo == SIGINT || signo == SIGTERM){
        exit(EXIT_SUCCESS);
    }
}

void create_client_processes(){
    for(int i=0; i<client_count; i++){
        ClientInfo* client = &clients[i];

        pid_t pid = fork();

        if(pid == -1){
            perror("Error creating client process");
            continue;
        } else if(pid == 0){
            //Child process
            signal(SIGINT, handle_signals_child);
            signal(SIGTERM, handle_signals_child);

            client_process(client);
            exit(EXIT_SUCCESS);
        } else {
            //Parent process
            client->connected = 1; // Mark as connected
        }

        //Parent continues to create other clients
    }
}

void client_process(ClientInfo* client){
    //Create client FIFO
    char client_fifo[MAX_PATH_LENGTH];
    snprintf(client_fifo, sizeof(client_fifo), "/tmp/client%d_fifo", client->client_num);
    //printf("Client FIFO: %s and I am client %d\n", client_fifo, getpid());

    // Remove existing FIFO if any
    unlink(client_fifo);
    
    if(mkfifo(client_fifo, 0666) == -1){
        perror("Error creating client FIFO");
        return;
    }

    //Open client FIFO for reading and writing
    int client_fifo_fd = open(client_fifo, O_WRONLY);
    if(client_fifo_fd == -1){
        perror("Error opening client FIFO");
        unlink(client_fifo);
        return;
    }

    //Print connection message
    printf("Client%02d connected.. %s %s %d credits\n", 
        client->client_num,
        client->transaction.account_id, 
        client->transaction.operation == DEPOSIT ? "depositing" : "withdrawing", 
        client->transaction.amount);

    //Send transaction request to teller
    write(client_fifo_fd, &client->transaction, sizeof(client->transaction));

    //Close client FIFO
    close(client_fifo_fd);

    //Open client FIFO for reading
    client_fifo_fd = open(client_fifo, O_RDONLY);
    if(client_fifo_fd == -1){
        perror("Error opening client FIFO for reading");
        unlink(client_fifo);
        return;
    }

    //Wait for response from teller
    char response[MAX_BUFFER_SIZE];
    ssize_t bytes_read = read(client_fifo_fd, response, sizeof(response));
    if(bytes_read > 0){
        //Process response
        response[bytes_read] = '\0'; // Null-terminate the response
        printf("Client%02d served. %s\n", client->client_num, response);
    } else if(bytes_read == -1){
        perror("Error reading from client FIFO");
    }

    close(client_fifo_fd);
    unlink(client_fifo); // Remove FIFO after use
    client->connected = 0; // Mark as disconnected
    //printf("Client%02d disconnected.\n", client->client_num);
}

void cleanup_client(){
    //Reset global variables
    memset(clients, 0, sizeof(clients));
    client_count = 0;
    running = 0;
}