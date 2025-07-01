#include "bankServer.h"

// Global variables
BankDatabase bank_db;
char bank_name[MAX_PATH_LENGTH];
char server_fifo_name[MAX_PATH_LENGTH];
int server_fifo_fd = -1;
int running = 1;
pid_t teller_pids[MAX_CLIENTS];
int teller_count = 0;

// Shared memory and semaphore IDs for each teller
int shm_ids[MAX_CLIENTS];
int sem_ids[MAX_CLIENTS];

int log_sem_id = -1;

int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <bank_name> <server_fifo_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    initialize_server(argv[1], argv[2]);

    printf("%s is active...\n", bank_name);

    initialize_database();

    //Set up signal handlers
    signal(SIGINT, handle_signals);
    signal(SIGTERM, handle_signals);

    // Main server loop
    process_client_connections();

    //Clean up before exiting
    cleanup_server();
    printf("%s says \"Bye\"...\n", bank_name);

    return 0;
}

void process_client_connections(){
    while(running){
        printf("Waiting for clients @%s...\n", server_fifo_name);
        // Structure to receive client connection requests
        struct {
            pid_t client_pid;
            int client_count;
            char operation[MAX_CLIENTS];
        } client_req;

        // Reopen the FIFO for each new batch of clients
        if (server_fifo_fd == -1) {
            server_fifo_fd = open_server_fifo(server_fifo_name, O_RDONLY | O_NONBLOCK);
            if (server_fifo_fd == -1) {
                perror("reopen server fifo");
                sleep(1);
                continue;
            }
        }

        //Wait for a client connection request
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_fifo_fd, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 2; // 2 seconds timeout
        timeout.tv_usec = 0;

        int ready = select(server_fifo_fd + 1, &read_fds, NULL, NULL, &timeout);

        if(ready == -1){
            if(errno != EINTR){
                perror("select");
            }
            continue;
        } else if(ready == 0){
            // Timeout occurred, no clients connected
            continue;
        }

        ssize_t bytes_read = read(server_fifo_fd, &client_req, sizeof(client_req));

        if(bytes_read == -1){
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("read server fifo");
                close(server_fifo_fd);
                server_fifo_fd = -1;
            }
            continue;
        } else if(bytes_read == 0){
            // EOF reached, no clients connected
            close(server_fifo_fd);
            server_fifo_fd = -1;
            continue;
        } else if(bytes_read != sizeof(client_req)){
            fprintf(stderr, "Invalid request size from client.\n");
            continue;
        }
        
        // Process client connection request
        printf("- Received %d clients from PIDClient%d..\n", client_req.client_count, client_req.client_pid);
        
        for(int i = 0; i<client_req.client_count && i < MAX_CLIENTS; i++){
            create_teller(client_req.client_pid , (i + 1), client_req.operation[i]);
        }

        //Process any teller requests(via shared memory)
        process_teller_requests();

        for (int i = 0; i < teller_count; i++) {
            if (teller_pids[i] > 0) {
                int status;
                int result = waitTeller(teller_pids[i], &status);
                if (result != -1) {
                    if (!(WIFEXITED(status) || WIFSIGNALED(status))) {
                        printf("Teller PID %d exited abnormally.\n", teller_pids[i]);
                    }
                    teller_pids[i] = -1;
                }
            }
        }
    }
}

//Process requests from tellers via shared memory
void process_teller_requests(){
    for(int i = 0; i<teller_count; i++){
        if(teller_pids[i] == -1 || shm_ids[i] == -1 || sem_ids[i] == -1){
            //fprintf(stderr, "Invalid teller PID or shared memory ID for teller %d\n", i);   
            continue;
        }

        //Attach to shared memory segment
        SharedMemorySegment* shm = attach_shared_memory(shm_ids[i]);
        if(shm == NULL){
            fprintf(stderr, "Failed to attach shared memory for teller PID_Client%d_%dhereee\n", teller_pids[i], i);
            continue;
        }
        sem_operation(sem_ids[i], SEM_LOCK);
        //something wrong
        if(shm->request_ready == -1){
            sem_operation(sem_ids[i], SEM_UNLOCK);

            detach_shared_memory(shm);
            cleanup_shared_memory(shm_ids[i]);
            cleanup_semaphore(sem_ids[i]);
            continue; // No request to process
        }

        sem_operation(sem_ids[i], SEM_UNLOCK);
        
        //printf("my shm id is %d and sem id is %d in process teller\n", shm_ids[i], sem_ids[i]);
        //Check if there's a request from the teller
        while(running){
            //Check if there's a request from the teller
            sem_operation(sem_ids[i], SEM_LOCK);
            
            if(shm->request_ready){
                //Process the request
                Request* request = &shm->request;
                Request* response = &shm->response;

                //Copy request to response
                memcpy(response, request, sizeof(Request));

                //Process the request
                response->status = process_transaction(response);

                // Set appropriate message based on status
                switch (response->status) {
                    case SUCCESS:
                        if (response->operation == 'D') {
                            snprintf(response->message, sizeof(response->message), 
                                    "Deposit of %d credits successful", response->amount);
                        } else if (response->operation == 'W') {
                            snprintf(response->message, sizeof(response->message), 
                                    "Withdrawal of %d credits successful", response->amount);
                        }
                        break;
                    case ERR_INSUFFICIENT_FUNDS:
                        snprintf(response->message, sizeof(response->message), 
                                "Insufficient funds for withdrawal");
                        break;
                    case ERR_INVALID_ACCOUNT:
                        snprintf(response->message, sizeof(response->message), 
                                "Invalid account ID");
                        break;
                    case ERR_INVALID_OPERATION:
                        snprintf(response->message, sizeof(response->message), 
                                "Invalid operation or amount");
                        break;
                    default:
                        snprintf(response->message, sizeof(response->message), 
                                "System error");
                        break;
                }

                //Mark the request as processed
                shm->request_ready = 0;
                shm->response_ready = 1;
                sem_operation(sem_ids[i], SEM_UNLOCK);

                break;
            }

            //Unlock semaphore
            sem_operation(sem_ids[i], SEM_UNLOCK);

            //Check if the teller is still running
            if (teller_pids[i] == -1) {
                break; // Teller process has exited
            }
            
            //Wait a bit before checking again
            usleep(10000); // 10ms
        }
        
        //Detach shared memory
        detach_shared_memory(shm);
    }
}

int process_transaction(Request* request){
    if(request->operation == 'D'){
        if(strcmp(request->account_id, "N") == 0){
            // Create a new account
            if(bank_db.account_count >= MAX_ACCOUNTS){
                return ERR_SYSTEM_ERROR;
            }
            BankAccount* new_account = &bank_db.accounts[bank_db.account_count++];
            new_account->account_id = bank_db.next_account_id++;
            snprintf(request->account_id, sizeof(request->account_id), "BankID_%02d", new_account->account_id);
            new_account->balance = request->amount;
            new_account->transaction_count = 1;
            new_account->transactions[0].operation = 'D';
            new_account->transactions[0].amount = request->amount;
            //Update log file
            update_log_file();

            return SUCCESS;
        } else {
            // Deposit to existing account
            BankAccount* account = find_account(request->account_id);
            if(account == NULL ){
                return ERR_INVALID_ACCOUNT;
            }
            if(account->balance == 0){
                return ERR_INVALID_ACCOUNT;
            }

            //Update account balance
            account->balance += request->amount;
            if(account->transaction_count < MAX_TRANSACTION){
                account->transactions[account->transaction_count].operation = 'D';
                account->transactions[account->transaction_count].amount = request->amount;
                account->transaction_count++;
            } else {
                fprintf(stderr, "Transaction limit reached for account %s.\n", request->account_id);
            }
            
            //Update log file
            update_log_file();

            return SUCCESS;
        }
    } else if(request->operation == 'W'){
        //Handle withdrawal
        BankAccount* account = find_account(request->account_id);
        if(account == NULL){
            return ERR_INVALID_ACCOUNT;
        }
        if(account->balance == 0){
            return ERR_INVALID_ACCOUNT;
        }
        //Check if sufficient funds are available
        if(account->balance < request->amount){
            return ERR_INSUFFICIENT_FUNDS;
        }

        //Update account balance
        account->balance -= request->amount;
        if(account->transaction_count < MAX_TRANSACTION){
            account->transactions[account->transaction_count].operation = 'W';
            account->transactions[account->transaction_count].amount = request->amount;
            account->transaction_count++;
        } else {
            fprintf(stderr, "Transaction limit reached for account %s.\n", request->account_id);
            return ERR_SYSTEM_ERROR;
        }

        //If balance is zero, mark account as inactive
        if(account->balance == 0){
            snprintf(request->message, sizeof(request->message), "Account closed");
        } else {
            snprintf(request->message, sizeof(request->message), "Withdrawal successful");
        }

        //Update log file
        update_log_file();

        return SUCCESS;
    } else {
        //Invalid operation
        return ERR_INVALID_OPERATION;
    }
}

void create_teller(int client_pid, int client_no, char client_operation){
    if(teller_count >= MAX_CLIENTS){
        fprintf(stderr, "Maximum teller count reached\n");
        return;
    }

    //Create shared memory segment for this teller
    int shm_id = create_shared_memory();
    if(shm_id == -1){
        fprintf(stderr, "Failed to create shared memory for teller to server PID_Client%d %d\n", client_pid, client_no);
        return;
    }

    // Create semaphore for synchronization
    int sem_id = init_semaphore();
    if(sem_id == -1){
        fprintf(stderr, "Failed to create semaphore for teller to server PID_Client%d %d\n", client_pid, client_no);
        cleanup_shared_memory(shm_id);
        return;
    }

    // Initialize shared memory
    SharedMemorySegment* shm = attach_shared_memory(shm_id);
    if(shm == NULL){
        fprintf(stderr, "Failed to attach shared memory for teller to server PID_Client%d %d\n", client_pid, client_no);
        cleanup_shared_memory(shm_id);
        cleanup_semaphore(sem_id);
        return;
    }

    // Initialize shared memory values
    shm->request_ready = 0;
    shm->response_ready = 0;
    memset(&shm->request, 0, sizeof(Request));
    memset(&shm->response, 0, sizeof(Request));

    detach_shared_memory(shm);

    //Store shared memory and semaphore IDs
    shm_ids[teller_count] = shm_id;
    sem_ids[teller_count] = sem_id;

    //Preapare arguments for teller function
    typedef struct{
        int client_pid;
        int client_no;
        int shm_id;
        int sem_id;
    } TellerArgs;

    TellerArgs* args = malloc(sizeof(TellerArgs));
    args->client_pid = client_pid;
    args->client_no = client_no;
    args->shm_id = shm_id;
    args->sem_id = sem_id;

    pid_t teller_pid;
    if(client_operation == 'D'){
        teller_pid = Teller((void*)deposit, args);
    } else if(client_operation == 'W'){
        teller_pid = Teller((void *)withdraw, args);
    } else {
        fprintf(stderr, "Invalid operation for teller %d\n", client_no);
        free(args);
        cleanup_semaphore(sem_id);
        cleanup_shared_memory(shm_id);
        return;
    }

    if(teller_pid > 0){
        // Store teller PID
        teller_pids[teller_count] = teller_pid;
        teller_count++;
        printf("-- Teller PID%d is active serving Client%02d...\n", teller_pid, client_no);

        free(args);
    } else {
        fprintf(stderr, "Failed to create teller process for client %d\n", client_no);
        free(args);
        cleanup_semaphore(sem_id);
        cleanup_shared_memory(shm_id);
        return;
    }

}

void handle_signals_child(int signo){
    if(signo == SIGINT || signo == SIGTERM){
        running = 0;
    }
}

pid_t Teller(void* func, void* arg_func){
    pid_t pid = fork();

    if(pid < 0){
        //Fork failed
        perror("fork");
        return -1;
    } else if(pid == 0){
        //Child process(Teller)
        signal(SIGINT, handle_signals_child);
        signal(SIGTERM, handle_signals_child);
        //Close server FIFO, we don't need it in the child process
        if (server_fifo_fd != -1) {
            if (close(server_fifo_fd) == -1) {
                perror("close server_fifo_fd");
            }
            server_fifo_fd = -1;
        }

        OperationFunc operation_func = (OperationFunc)func;
        int result = operation_func(arg_func);

        exit(result);
    }

    //Parent process
    //Teller Pid is returned
    return pid;
}

int deposit(void* args){
    typedef struct{
        int client_pid;
        int client_no;
        int shm_id;
        int sem_id;
    } TellerArgs;

    TellerArgs* teller_args = (TellerArgs*)args;
    int client_pid = teller_args->client_pid;
    int client_no = teller_args->client_no;
    int shm_id = teller_args->shm_id;
    int sem_id = teller_args->sem_id;

    //Free the arguments as we don't need them anymore
    free(teller_args);

    //Preapare client FIFO name
    char client_fifo[MAX_PATH_LENGTH];
    snprintf(client_fifo, sizeof(client_fifo), "/tmp/client%d_fifo", client_no);
    //printf("I am %d teller and I am serving client %d via %s\n", getpid(), client_no, client_fifo);

    //Open client FIFO for read/write
    int client_fifo_fd = open(client_fifo, O_RDONLY);
    if (client_fifo_fd == -1) {
        perror("open client fifo");
        return ERR_SYSTEM_ERROR;
    }
    //Attach shared memory
    SharedMemorySegment* shm = attach_shared_memory(shm_id);
    if(shm == NULL){
        fprintf(stderr, "Failed to attach shared memory for teller PID_Client%d_%d\n", client_pid, client_no);
        close(client_fifo_fd);
        cleanup_semaphore(sem_id);
        cleanup_shared_memory(shm_id);
        return ERR_SYSTEM_ERROR;
    }

    //Read request from client
    Request request;
    ssize_t bytes_read = read(client_fifo_fd, &request, sizeof(Request));
    //printf("I am %d teller and I am serving client %d and request is '%c' bankID is '%s' amount is '%d' \n", getpid(), client_no, request.operation, request.account_id, request.amount);
    if(bytes_read != sizeof(Request)){
        fprintf(stderr, "Invalid request size from PID_Client%d_%d FIFO\n", client_pid, client_no);
        close(client_fifo_fd);
        shm->request_ready = -1;
        detach_shared_memory(shm);
        return ERR_SYSTEM_ERROR;
    }

    close(client_fifo_fd);

    //Check if request is valid
    if(request.operation != 'D'){
        fprintf(stderr, "Invalid operation in request from PID_Client%d_%d FIFO\n", client_pid, client_no);
        close(client_fifo_fd);
        sem_operation(sem_id, SEM_LOCK);
        shm->request_ready = -1;
        sem_operation(sem_id, SEM_UNLOCK);
        detach_shared_memory(shm);
        return ERR_SYSTEM_ERROR;
    }
    if(!is_valid_bank_id(request.account_id)){
        if(request.operation == 'D'){
            printf("Client%02d deposited %d credits. Operation not permitted.\n", client_no, request.amount);
        } else if(request.operation == 'W'){
            printf("Something went wrong.\n");
        }
        sem_operation(sem_id, SEM_LOCK);
        shm->request_ready = -1;
        sem_operation(sem_id, SEM_UNLOCK);
        detach_shared_memory(shm);

        char client_response[MAX_BUFFER_SIZE];
        
        client_fifo_fd = open(client_fifo, O_WRONLY);
        snprintf(client_response, sizeof(client_response), "Invalid Bank ID!");
        
        if (client_fifo_fd == -1) {
            perror("open client fifo");
            return ERR_SYSTEM_ERROR;
        }

        //Send response to client
        write(client_fifo_fd, client_response, sizeof(client_response));

        close(client_fifo_fd);

        return ERR_SYSTEM_ERROR;
    }
    if(request.amount <= 0){
        fprintf(stderr, "Invalid amount in request from PID_Client%d_%d FIFO\n", client_pid, client_no);
        close(client_fifo_fd);
        sem_operation(sem_id, SEM_LOCK);
        shm->request_ready = -1;
        sem_operation(sem_id, SEM_UNLOCK);
        detach_shared_memory(shm);

        char client_response[MAX_BUFFER_SIZE];
        
        client_fifo_fd = open(client_fifo, O_WRONLY);
        snprintf(client_response, sizeof(client_response), "Invalid amount request!");
        
        if (client_fifo_fd == -1) {
            perror("open client fifo");
            return ERR_SYSTEM_ERROR;
        }

        //Send response to client
        write(client_fifo_fd, client_response, sizeof(client_response));

        close(client_fifo_fd);

        return ERR_SYSTEM_ERROR;
    }

    //Lock semaphore
    sem_operation(sem_id, SEM_LOCK);

    //Copy request to shared memory
    memcpy(&shm->request, &request, sizeof(Request));
    shm->request_ready = 1;
    shm->response_ready = 0;

    //Unlock semaphore
    sem_operation(sem_id, SEM_UNLOCK);

    //Wait for server to process the request
    while(running) {
        //Lock semaphore
        sem_operation(sem_id, SEM_LOCK);

        if(shm->response_ready == 1){
            //Copy response from shared memory

            Request response = shm->response;

            //Reset response ready flag
            shm->response_ready = 0;

            //Unlock semaphore
            sem_operation(sem_id, SEM_UNLOCK);

            //Process response
            char client_response[MAX_BUFFER_SIZE];
            if(response.status == SUCCESS){
                if(strcmp(request.account_id, "N") == 0){
                    //New account created
                    printf("Client%02d deposited %d credits to account %s.\n", client_no, request.amount, response.account_id);
                    snprintf(client_response, sizeof(client_response), "%s", response.account_id);
                } else {
                    //Existing account
                    printf("Client%02d deposited %d credits to account %s.\n", client_no, request.amount, request.account_id);
                    snprintf(client_response, sizeof(client_response), "Deposit successful");
                }
            } else {
                //Error occurred
                printf("Client%02d deposit failed for account %s.\n", client_no, request.account_id);
                snprintf(client_response, sizeof(client_response), "Deposit failed: %s", response.message);
            }

            client_fifo_fd = open(client_fifo, O_WRONLY);

            if (client_fifo_fd == -1) {
                perror("open client fifo");
                sem_operation(sem_id, SEM_LOCK);
                shm->request_ready = -1;
                sem_operation(sem_id, SEM_UNLOCK);
                detach_shared_memory(shm);
                cleanup_semaphore(sem_id);
                return ERR_SYSTEM_ERROR;
            }

            //Send response to client
            write(client_fifo_fd, client_response, sizeof(client_response));

            close(client_fifo_fd);
            
            break;
        }

        //Unlock semaphore
        sem_operation(sem_id, SEM_UNLOCK);

        //Wait a bit before checking again
        usleep(10000); // 10ms
    }

    // Clean up
    //close(client_fifo_fd);
    detach_shared_memory(shm);
    cleanup_semaphore(sem_id);
    cleanup_shared_memory(shm_id);

    return SUCCESS;
}

int withdraw(void *args){
    typedef struct{
        int client_pid;
        int client_no;
        int shm_id;
        int sem_id;
    } TellerArgs;

    TellerArgs* teller_args = (TellerArgs*)args;
    int client_pid = teller_args->client_pid;
    int client_no = teller_args->client_no;
    int shm_id = teller_args->shm_id;
    int sem_id = teller_args->sem_id;

    //Free the arguments as we don't need them anymore
    free(teller_args);

    //Preapere client FIFO name
    char client_fifo[MAX_PATH_LENGTH];
    snprintf(client_fifo, sizeof(client_fifo), "/tmp/client%d_fifo", client_no);
    //printf("I am %d teller and I am serving client %d via %s\n", getpid(), client_no, client_fifo);
    
    //Open client FIFO for read/write
    int client_fifo_fd = open(client_fifo, O_RDONLY);
    if (client_fifo_fd == -1) {
        perror("open client fifo");
        return ERR_SYSTEM_ERROR;
    }

    //Attach shared memory
    SharedMemorySegment* shm = attach_shared_memory(shm_id);
    if(shm == NULL){
        fprintf(stderr, "Failed to attach shared memory for teller PID_Client%d_%d\n", client_pid, client_no);
        close(client_fifo_fd);
        cleanup_semaphore(sem_id);
        cleanup_shared_memory(shm_id);
        return ERR_SYSTEM_ERROR;
    }
    //Read request from client
    Request request;
    ssize_t bytes_read = read(client_fifo_fd, &request, sizeof(Request));
    if(bytes_read != sizeof(Request)){
        fprintf(stderr, "Invalid request size from PID_Client%d_%d FIFO\n", client_pid, client_no);
        close(client_fifo_fd);
        sem_operation(sem_id, SEM_LOCK);
        shm->request_ready = -1;
        sem_operation(sem_id, SEM_UNLOCK);
        detach_shared_memory(shm);
        return ERR_SYSTEM_ERROR;
    }
    //Check if request is valid
    if(request.operation != 'W'){
        fprintf(stderr, "Invalid operation in request from PID_Client%d_%d FIFO\n", client_pid, client_no);
        close(client_fifo_fd);
        sem_operation(sem_id, SEM_LOCK);
        shm->request_ready = -1;
        sem_operation(sem_id, SEM_UNLOCK);
        detach_shared_memory(shm);
        return ERR_SYSTEM_ERROR;
    }
    if(!is_valid_bank_id(request.account_id)){
        if(request.operation == 'W'){
            printf("Client%02d withdraws %d credits. Operation not permitted.\n", client_no, request.amount);
        } else if(request.operation == 'D'){
            printf("Something went wrong.\n");
        }
        sem_operation(sem_id, SEM_LOCK);
        shm->request_ready = -1;
        sem_operation(sem_id, SEM_UNLOCK);
        detach_shared_memory(shm);

        char client_response[MAX_BUFFER_SIZE];
        client_fifo_fd = open(client_fifo, O_WRONLY);
        snprintf(client_response, sizeof(client_response), "Invalid Bank ID!");
        
        if (client_fifo_fd == -1) {
            perror("open client fifo");
            return ERR_SYSTEM_ERROR;
        }

        //Send response to client
        write(client_fifo_fd, client_response, sizeof(client_response));

        close(client_fifo_fd);

        return ERR_SYSTEM_ERROR;
    }
    if(request.amount <= 0){
        fprintf(stderr, "Invalid amount in request from PID_Client%d_%d FIFO\n", client_pid, client_no);
        close(client_fifo_fd);
        sem_operation(sem_id, SEM_LOCK);
        shm->request_ready = -1;
        sem_operation(sem_id, SEM_UNLOCK);
        detach_shared_memory(shm);

        char client_response[MAX_BUFFER_SIZE];
        
        client_fifo_fd = open(client_fifo, O_WRONLY);
        snprintf(client_response, sizeof(client_response), "Invalid amount in request!");
        
        if (client_fifo_fd == -1) {
            perror("open client fifo");
            return ERR_SYSTEM_ERROR;
        }

        //Send response to client
        write(client_fifo_fd, client_response, sizeof(client_response));

        close(client_fifo_fd);

        return ERR_SYSTEM_ERROR;
    }
    close(client_fifo_fd);

    //Lock semaphore
    sem_operation(sem_id, SEM_LOCK);

    //Copy request to shared memory
    memcpy(&shm->request, &request, sizeof(Request));
    shm->request_ready = 1;
    shm->response_ready = 0;

    //Unlock semaphore
    sem_operation(sem_id, SEM_UNLOCK);

    //Wait for server to process the request
    while(running) {
        //Lock semaphore
        sem_operation(sem_id, SEM_LOCK);

        if(shm->response_ready == 1){
            //Copy response from shared memory

            Request response = shm->response;

            //Reset response ready flag
            shm->response_ready = 0;

            //Unlock semaphore
            sem_operation(sem_id, SEM_UNLOCK);

            //Process response
            char client_response[MAX_BUFFER_SIZE];
            if(response.status == SUCCESS){
                if(strcmp(response.message, "account closed") == 0){
                    //Account closed
                    printf("Client%02d withdrew %d credits from account %s.\n", client_no, request.amount, request.account_id);
                    snprintf(client_response, sizeof(client_response), "account closed");
                } else {
                    //Existing account
                    printf("Client%02d withdrew %d credits from account %s.\n", client_no, request.amount, request.account_id);
                    snprintf(client_response, sizeof(client_response), "%s", response.account_id);
                }
            } else {
                //Error occurred
                printf("Client%02d withdraw failed for account %s.\n", client_no, request.account_id);
                snprintf(client_response, sizeof(client_response), "%s", response.message);
            }

            //Reopen client FIFO for writing
            client_fifo_fd = open(client_fifo, O_WRONLY);
            
            //Send response to client
            write(client_fifo_fd, client_response, sizeof(client_response));
            
            //close client FIFO
            close(client_fifo_fd);

            break;
        }

        //Unlock semaphore
        sem_operation(sem_id, SEM_UNLOCK);

        //Wait a bit before checking again
        usleep(10000); // 10ms
    }

    // Clean up
    //close(client_fifo_fd);
    detach_shared_memory(shm);
    cleanup_semaphore(sem_id);
    cleanup_shared_memory(shm_id);
    
    return SUCCESS;
}

int is_valid_bank_id(const char* bank_id){
    // Check if the Bank_ID starts with "BankID_"
    if(strcmp(bank_id , "N") == 0){
        return 1; // Valid Bank_ID
    }else if (strncmp(bank_id, "BankID_", 7) == 0) {
        // Ensure the rest of the string is a valid number or specific character
        const char* suffix = bank_id + 7; // Skip "BankID_"
        if (isdigit(suffix[0])) {
            return 1; // Valid Bank_ID
        }
    }
    return 0; // Invalid Bank_ID
}

//Shared memory creation function
int create_shared_memory(){
    key_t key = IPC_PRIVATE;
    int shm_id = shmget(key, sizeof(SharedMemorySegment), IPC_CREAT | 0666);

    if(shm_id == -1){
        perror("shmget");
        return -1;
    }

    return shm_id;
}

//Semaphore initialization function
int init_semaphore(){
    key_t key = IPC_PRIVATE;
    int sem_id = semget(key, 1, IPC_CREAT | 0666);

    if(sem_id == -1){
        perror("semget");
        return -1;
    }

    // Initialize semaphore to 1 (unlocked)
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;

    argument.val = 1;
    if(semctl(sem_id, 0, SETVAL, argument) == -1){
        perror("semctl");
        semctl(sem_id, 0, IPC_RMID, 0);
        return -1;
    }

    return sem_id;
}

int transform_BankID_to_int(const char* bank_id){
    if(strcmp(bank_id , "N") == 0){
        return -1; // Invalid Bank_ID
    }else if (strncmp(bank_id, "BankID_", 7) == 0) {
        // Ensure the rest of the string is a valid number or specific character
        const char* suffix = bank_id + 7; // Skip "BankID_"
        return atoi(suffix); // Convert to integer
    }
    return -1; // Invalid Bank_ID
}

//Semaphore cleanup function
void cleanup_shared_memory(int shm_id) {
    if (shm_id != -1) {
        if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
            perror("shmctl IPC_RMID");
        }
        for(int i = 0; i < MAX_CLIENTS; i++) {
            if (shm_ids[i] == shm_id) {
                shm_ids[i] = -1; // Mark as cleaned up
            }
        }
    }
}

void handle_signals(int signo){
    if(signo == SIGINT || signo == SIGTERM){
        const char* msg = "\nSignal received, closing...\n";
        write(STDOUT_FILENO, msg, strlen(msg));

        //wait child processes
        for (int i = 0; i < teller_count; i++) {
            if (teller_pids[i] > 0) {
                int status;
                int result = waitTeller(teller_pids[i], &status);
                if (result != -1) {
                    printf("Teller PID %d finished (non-blocking).\n", teller_pids[i]);
                    if (WIFEXITED(status)) {
                        printf("Teller PID %d exited normally with status %d.\n", teller_pids[i], WEXITSTATUS(status));
                    } else if (WIFSIGNALED(status)) {
                        printf("Teller PID %d terminated by signal %d.\n", teller_pids[i], WTERMSIG(status));
                    } else {
                        printf("Teller PID %d exited abnormally.\n", teller_pids[i]);
                    }
                    teller_pids[i] = -1;
                }
            }
        }

        running = 0;
    }
}

int waitTeller(pid_t pid, int* status){
    int stat;
    pid_t result = waitpid(pid, &stat, 0);

    if (result == -1) return -1;  // waitpid failed
    
    if (status != NULL) {
        *status = WEXITSTATUS(stat);
    }
    
    return result;
}

void initialize_database(){
    // Try to load existing database or create a new one
    if (access(bank_db.log_filename, F_OK) != -1) {
        printf("Loading bank database from log...\n");
        load_database_from_log();
    } else {
        printf("No previous logs, creating the bank database...\n");
        bank_db.account_count = 0;
        bank_db.next_account_id = 1;
    }
}

void load_database_from_log(){
    FILE* log_file = fopen(bank_db.log_filename, "r");
    if (log_file == NULL) {
        perror("fopen log file");
        return;
    }
    
    char line[MAX_BUFFER_SIZE];
    bank_db.account_count = 0;
    bank_db.next_account_id = 1;
    
    while (fgets(line, sizeof(line), log_file) != NULL) {
        // Skip comment lines
        if (line[0] == '#') {
            //printf("Skipping comment line: %s", line);
            continue;
        }
        
        // Parse account information
        char account_id[20];
        
        char* token = strtok(line, " ");
        if(token != NULL){
            strncpy(account_id, token, sizeof(account_id) - 1);
            account_id[sizeof(account_id) - 1] = '\0';

            BankAccount* account = find_account(account_id);
            if(account == NULL && bank_db.account_count < MAX_ACCOUNTS){
                // Create a new account if it doesn't exist
                account = &bank_db.accounts[bank_db.account_count++];
                account->account_id = transform_BankID_to_int(account_id);
                if(account->account_id >= bank_db.next_account_id){
                    bank_db.next_account_id = account->account_id + 1;
                }
                account->balance = 0; // Initialize balance to 0
                account->transaction_count = 0;
            }

            while((token = strtok(NULL, " ")) != NULL){
                if(strcmp(token, "D") == 0 || strcmp(token, "W") == 0){
                    if(account->transaction_count < MAX_TRANSACTION){
                        account->transactions[account->transaction_count].operation = token[0]; // Next token is the operation (D/W)
                        token = strtok(NULL, " "); // get the amount
                        if (token != NULL) {
                            account->transactions[account->transaction_count].amount = atoi(token); // Assign the amount
                            account->transaction_count++; // Increment the transaction count
                        } else {
                            fprintf(stderr, "Invalid transaction format: missing amount.\n");
                        }
                    } else {
                        fprintf(stderr, "Transaction limit reached for account %s.\n", account_id);
                    }
                } else {
                    account->balance = atoi(token); // Assign the balance
                }
            }
        }
    }
    
    fclose(log_file);
}

BankAccount* find_account(const char* account_id) {
    int account_id_int = transform_BankID_to_int(account_id);
    for (int i = 0; i < bank_db.account_count; i++) {
        if (account_id_int == bank_db.accounts[i].account_id) {
            return &bank_db.accounts[i];
        }
    }
    return NULL;
}

void initialize_server(const char* name, const char* fifo_name){
    strncpy(bank_name, name, MAX_PATH_LENGTH - 1);
    strncpy(server_fifo_name, fifo_name, MAX_PATH_LENGTH - 1);

    //printf("%s %s\n", name, fifo_name);

    // Initialize bank database
    memset(&bank_db, 0, sizeof(BankDatabase));
    snprintf(bank_db.log_filename, sizeof(bank_db.log_filename), "%s.bankLog", bank_name);
    bank_db.last_update = time(NULL);

    unlink(server_fifo_name); // Remove any existing FIFO with the same name

    //Create server FIFO if it doesn't exist
    if (access(server_fifo_name, F_OK) == -1) {
        if (mkfifo(server_fifo_name, 0666) == -1) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    // Open server FIFO for reading
    server_fifo_fd = open_server_fifo(server_fifo_name, O_RDONLY | O_NONBLOCK);
    if(server_fifo_fd == -1){
        perror("open server fifo");
        exit(EXIT_FAILURE);
    }

    // Initialize teller tracking and shared memory arrays
    memset(teller_pids, 0, sizeof(teller_pids));
    memset(shm_ids, -1, sizeof(shm_ids));
    memset(sem_ids, -1, sizeof(sem_ids));
    teller_count = 0;

    // Initialize log semaphore
    log_sem_id = init_semaphore();
    if (log_sem_id == -1) {
        fprintf(stderr, "Failed to initialize log file semaphore\n");
        exit(EXIT_FAILURE);
    }
}

int open_server_fifo(const char* fifo_name, int mode){
    int fd = open(fifo_name, mode);
    if(fd == -1){
        perror("open fifo");
    }

    return fd;
}

void cleanup_semaphore(int sem_id){
    if(sem_id != -1){
        if(semctl(sem_id, 0, IPC_RMID, 0) == -1){
            perror("semctl IPC_RMID");
        }
        for(int i = 0; i < MAX_CLIENTS; i++){
            if(sem_ids[i] == sem_id){
                sem_ids[i] = -1; // Mark as cleaned up
            }
        }
    }
}

void detach_shared_memory(SharedMemorySegment* shm){
    if(shm != NULL && shm != (void*)-1){
        if(shmdt(shm) == -1){
            perror("shmdt");
        }
    }
}

SharedMemorySegment* attach_shared_memory(int shm_id){
    SharedMemorySegment* shm = (SharedMemorySegment*)shmat(shm_id, NULL, 0);
    if(shm == (void*)-1){
        perror("shmat");
        return NULL;
    }

    return shm;
}

void cleanup_server(){
    // Close server FIFO
    if (server_fifo_fd != -1) {
        printf("Cleaning up server FIFO...\n");
        close(server_fifo_fd);
        server_fifo_fd = -1;
    }

    if (access(server_fifo_name, F_OK) != -1) {
        if (unlink(server_fifo_name) == -1) {
            perror("unlink server fifo");
        }
    }

    if (log_sem_id != -1) {
        cleanup_semaphore(log_sem_id);
        log_sem_id = -1;
    }

    // Update log file before exiting
    //printf("Updating log file...\n");
    //update_log_file();
}

void sem_operation(int sem_id, int op){
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = op;
    sb.sem_flg = 0;

    if(semop(sem_id, &sb, 1) == -1){
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

void update_log_file(){
    if (log_sem_id == -1) {
        fprintf(stderr, "Log semaphore not initialized!\n");
        return;
    }

    // Lock semaphore
    sem_operation(log_sem_id, SEM_LOCK);
    
    FILE* log_file = fopen(bank_db.log_filename, "w");
    if (log_file == NULL) {
        perror("fopen log file");
        return;
    }
    
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%H:%M %B %d %Y", timeinfo);
    
    fprintf(log_file, "# %s Log file updated @%s\n", bank_name, timestamp);
    
    // Write each active account to the log
    for (int i = 0; i < bank_db.account_count; i++) {
        BankAccount* account = &bank_db.accounts[i];
        if (account->balance != 0) {
            fprintf(log_file, "BankID_%02d ", account->account_id);
            for (int j = 0; j < account->transaction_count; j++) {
                fprintf(log_file, "%c %d ", account->transactions[j].operation, account->transactions[j].amount);
            }
            fprintf(log_file, "%d\n", account->balance);
        } else if(account->balance == 0) {
            // If the account has a balance of 0, write it as a comment
            fprintf(log_file, "#BankID_%02d ", account->account_id);
            for (int j = 0; j < account->transaction_count; j++) {
                fprintf(log_file, "%c %d ", account->transactions[j].operation, account->transactions[j].amount);
            }
            fprintf(log_file, "%d\n", account->balance);
        }
    }
    
    fprintf(log_file, "## end of log.\n");
    fclose(log_file);

    //Unlock semaphore for log file
    sem_operation(log_sem_id, SEM_UNLOCK); 
}