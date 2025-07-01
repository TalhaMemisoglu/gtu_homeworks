#include <stdio.h> //Just for snprintf, not for printf and scanf
#include <stdlib.h>
#include <string.h> //For string operations
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h> //For directory operations
#include <time.h> //For timestamp
#include <sys/wait.h> //For wait
#include <sys/file.h>  // For file operations

#define LOG_FILE "log.txt"

void createDir(char *folderName);
void createFile(char *fileName);
void listDir(char *folderName);
void listFilesByExtension(char *folderName, char *extension);
void readFile(char *fileName);
void appendToFile(char *fileName, char *content);
void deleteDir(char *folderName);
void deleteFile(char *fileName);
void showLogs();
void showHelp();
void logOperation(const char *message);

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_COUNT 5

int main(){

    char input[MAX_INPUT_SIZE];
    char *arguments[MAX_ARG_COUNT];

    int end = 0;
    while(end != 1) {
        // Read input using read
        write(STDOUT_FILENO, "> ", 2);
        ssize_t bytes_read = read(STDIN_FILENO, input, MAX_INPUT_SIZE - 1);
        if (bytes_read <= 0) {
            write(STDOUT_FILENO, "Error reading input.\n", 21);
            return 1;
        }
        
        input[bytes_read] = '\0';  // Add null terminator

        // Parse input with support for quoted arguments
        int arg_count = 0;
        char *p = input;
        
        while (*p != '\0' && arg_count < MAX_ARG_COUNT) {
            // Skip leading whitespace
            while (*p != '\0' && (*p == ' ' || *p == '\t' || *p == '\n')) p++;
            if (*p == '\0') break;
            
            if (*p == '"') {
                // Handle quoted argument
                p++; // Skip opening quote
                arguments[arg_count++] = p; // Start of argument
                
                // Find closing quote
                char *end = strchr(p, '"');
                if (end) {
                    *end = '\0'; // Terminate string at closing quote
                    p = end + 1; // Move past the closing quote
                } else {
                    // No closing quote found, treat rest of string as argument
                    p += strlen(p);
                }
            } else {
                // Regular argument
                arguments[arg_count++] = p;
                
                // Find end of argument (next whitespace)
                while (*p != '\0' && !(*p == ' ' || *p == '\t' || *p == '\n')) p++;
                if (*p != '\0') {
                    *p = '\0'; // Terminate argument
                    p++;       // Move to next character
                }
            }
        }
        
        // Null-terminate the arguments array
        arguments[arg_count] = NULL;

        if(strcmp(arguments[0], "fileManager") == 0){
            if(arg_count < 2){
                showHelp();
            } else if(strcmp(arguments[1], "createDir") == 0 && arg_count == 3) {
                createDir(arguments[2]);
            } else if (strcmp(arguments[1], "createFile") == 0 && arg_count == 3){
                createFile(arguments[2]);
            } else if(strcmp(arguments[1], "listDir") == 0 && arg_count == 3){
                listDir(arguments[2]);
            } else if(strcmp(arguments[1], "listFilesByExtension") == 0 && arg_count == 4){
                listFilesByExtension(arguments[2], arguments[3]);
            } else if (strcmp(arguments[1], "readFile") == 0 && arg_count == 3) {
                readFile(arguments[2]);
            } else if (strcmp(arguments[1], "appendToFile") == 0 && arg_count == 4) {
                appendToFile(arguments[2], arguments[3]);
            } else if (strcmp(arguments[1], "deleteFile") == 0 && arg_count == 3) {
                deleteFile(arguments[2]);
            } else if (strcmp(arguments[1], "deleteDir") == 0 && arg_count == 3) {
                deleteDir(arguments[2]);
            } else if(strcmp(arguments[1], "showLogs") == 0) {
                showLogs();
            } else if(strcmp(arguments[1], "exit") == 0 && arg_count == 2){
                end = 1;
            } else {
                showHelp();
            }
        } else {
            showHelp();
        }
    }
    
    return 0;
}

void createDir(char *folderName){
    //it creates a folder with full read, write and execute permissions to everyone
    //if directory already exist, print error.
    if(mkdir(folderName, 0777) == -1){
        char buffer[256];
        int length = snprintf(buffer, sizeof(buffer), "Error: Directory \"%s\" already exists.\n", folderName);
        write(STDOUT_FILENO, buffer, length);
    } else {
        char message[256];
        sprintf(message, "Directory \"%s\" created successfully.", folderName);
        logOperation(message); //for logging to log.txt
    }
}

void createFile(char *fileName) {
    //if file doesn't already exist, create a file.
    //"open function" creates a file with read & write permissions for everyone.
    int fdesc = open(fileName, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if(fdesc == -1){
        char buffer[256];
        int length = snprintf(buffer, sizeof(buffer), "Error: File \"%s\" already exists.\n", fileName);
        write(STDOUT_FILENO, buffer, length);
    } else {
        //write timestamp to the file.
        time_t t = time(NULL);
        char buffer[256];
        int length = snprintf(buffer, sizeof(buffer), "Created on: %s", ctime(&t));
        write(fdesc, buffer, length);
        close(fdesc);

        char message[256];
        sprintf(message, "File \"%s\" created succesfully.", fileName);
        logOperation(message);
    }
}

void listDir(char *folderName){
    //If the directory does not exist, program prints code below.
    DIR *dir = opendir(folderName);
    if (!dir) {
        char buffer[256];
        int length = snprintf(buffer, sizeof(buffer), "Error: Directory \"%s\" not found.\n", folderName);
        write(STDOUT_FILENO, buffer, length);
        return;
    }
    
    pid_t pid = fork();
   
    if(pid < 0){
        perror("Fork failed");
        closedir(dir);
        exit(EXIT_FAILURE);
    }

    if(pid == 0) { // Child process
        execlp("ls", "ls", "-la", folderName, NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else { // Parent process
        wait(NULL);
    }

    closedir(dir);
}

void listFilesByExtension(char *folderName, char *extension){
    DIR *dir = opendir(folderName);
    if (!dir) {
        char buffer[256];
        int length = snprintf(buffer, sizeof(buffer), "Error: Directory \"%s\" not found.\n", folderName);
        write(STDOUT_FILENO, buffer, length);
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        //for building command
        char command[256];
        snprintf(command, sizeof(command),
        "count=$(ls %s | grep '\\%s$' | wc -l); "
        "if [ \"$count\" -eq 0 ]; then "
        "echo \"No files with extension \\\"%s\\\" found in \\\"%s\\\".\"; "
        "else ls -la %s | grep '\\%s$'; fi",
        folderName, extension, extension, folderName, folderName, extension);

        //for executing our command that we built before.
        execlp("sh", "sh", "-c", command, NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else { // Parent process
        wait(NULL);
    }
}

void readFile(char *fileName) {
    int fd = open(fileName, O_RDONLY); // Open file in read-only mode
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytesRead); // Write directly to standard output
    }

    if (bytesRead == -1) {
        perror("Error reading file");
    }

    close(fd); // Close the file descriptor
}

void appendToFile(char *fileName, char *content) {
    int fd = open(fileName, O_WRONLY | O_APPEND);
    if (fd == -1) {
        write(STDERR_FILENO, "Error: Cannot write to file. File is locked or read-only.\n", 57);
        return;
    }

    // Locking
    if (flock(fd, LOCK_EX) == -1) {
        write(STDERR_FILENO, "Error: File is locked.\n", 24);
        close(fd);
        return;
    }

    write(fd, content, strlen(content));
    write(fd, "\n", 1); // Append newline

    char message[256];
    snprintf(message, sizeof(message), "File \"%s\" updated successfully.", fileName);
    logOperation(message);
    
    // Unlocking
    flock(fd, LOCK_UN);
    close(fd);
}

void deleteFile(char *fileName){
    if (access(fileName, F_OK) == -1) {
        char buffer[256];
        int length = snprintf(buffer, sizeof(buffer), "Error: File \"%s\" not found.\n", fileName);
        write(STDOUT_FILENO, buffer, length);
        return;
    }
    
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        execlp("rm", "rm", "-f", fileName, NULL);
        perror("execlp failed"); // Only runs if execlp fails
        exit(EXIT_FAILURE);
    } else { // Parent process
        wait(NULL);
        char message[256];
        sprintf(message, "File \"%s\" deleted successfully.", fileName);
        logOperation(message);
    }
}

void deleteDir(char *folderName){
    DIR *dir = opendir(folderName);
    if (!dir) {
        char buffer[256];
        int length = snprintf(buffer, sizeof(buffer), "Error: Directory \"%s\" not found.\n", folderName);
        write(STDOUT_FILENO, buffer, length);
        return;
    }

    struct dirent *entry;
    int fileCount = 0;

    // Check if directory is empty
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') { // Ignore "." and ".."
            fileCount++;
        }
    }
    closedir(dir);

    if (fileCount > 0) {
        char buffer[256];
        int length = snprintf(buffer, sizeof(buffer), "Error: Directory \"%s\" is not empty.\n", folderName);
        write(STDOUT_FILENO, buffer, length);
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        execlp("rm", "rm", "-r", folderName, NULL);
        perror("execlp failed"); // Only runs if execlp fails
        exit(EXIT_FAILURE);
    } else { // Parent process
        wait(NULL);
        char message[256];
        sprintf(message, "Directory \"%s\" deleted successfully.", folderName);
        logOperation(message);
    }
}

void logOperation(const char *message) {
    int fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    
    if (fd != -1) {
        // Get timestamp
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
        
        // Format the complete log message
        char logBuffer[512]; // Adjust size as needed
        int len = snprintf(logBuffer, sizeof(logBuffer), "[%s] %s\n", timeStr, message);
        
        // Write to file
        write(fd, logBuffer, len);
        
        // Close file descriptor
        close(fd);
    }
}

void showLogs() {
    readFile(LOG_FILE);
}

void showHelp(){
    const char helpText[] = 
        "Usage: fileManager <command> [arguments]\n"
        "Commands:\n"
        "  createDir \"folderName\"\n"
        "  createFile \"fileName\"\n"
        "  listDir \"folderName\"\n"
        "  listFilesByExtension \"folderName\" \".ext\"\n"
        "  readFile \"fileName\"\n"
        "  appendToFile \"fileName\" \"content\"\n"
        "  deleteFile \"fileName\"\n"
        "  deleteDir \"folderName\"\n"
        "  showLogs\n"
        "  exit\n";
    
    write(STDOUT_FILENO, helpText, strlen(helpText));
}