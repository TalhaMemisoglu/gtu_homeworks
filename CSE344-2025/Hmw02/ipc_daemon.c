#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define FIFO1 "/tmp/fifo1"
#define FIFO2 "/tmp/fifo2"
#define LOG_FILE "/tmp/daemon.log"
#define DAEMON_FILE "/tmp/daemon_fifo"

int child_counter = 0;
int num_children = 2; // We'll create two child processes

// Helper function to write strings to file descriptors
ssize_t write_str(int fd, const char *str) {
    return write(fd, str, strlen(str));
}

// Helper function to write integers to file descriptors
void write_int(int fd, int value) {
    char buffer[32];
    int len = snprintf(buffer, sizeof(buffer), "%d", value);
    write(fd, buffer, len);
}

// Function to handle SIGCHLD signal
void handle_sigchld() {
    pid_t pid;
    int status;
    char message[256];

    int fd = open(DAEMON_FILE, O_WRONLY | O_NONBLOCK);  // Non-blocking write
    if (fd < 0) {
        perror("open DAEMON_FILE in SIGCHLD");
        return;
    }

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        snprintf(message, sizeof(message),
                 "Child process %d terminated with status %d\n",
                 pid, WEXITSTATUS(status));

        write(fd, message, strlen(message));  // Write to daemon FIFO
        child_counter += 2;
    }

    close(fd);
}

void handle_daemon_signal(int sig) {
    time_t now = time(NULL);
    char *ts = ctime(&now);
    ts[strcspn(ts, "\n")] = 0; // remove newline

    char msg[256];

    switch (sig) {
        case SIGUSR1:
            snprintf(msg, sizeof(msg),
                     "[%s] [DAEMON] Received SIGUSR1: Custom user signal.\n", ts);
            break;
        case SIGHUP:
            snprintf(msg, sizeof(msg),
                     "[%s] [DAEMON] Received SIGHUP: Reloading configuration (not implemented).\n", ts);
            break;
        case SIGTERM:
            snprintf(msg, sizeof(msg),
                     "[%s] [DAEMON] Received SIGTERM: Shutting down gracefully.\n", ts);
            write(STDOUT_FILENO, msg, strlen(msg));
            _exit(0);  // use _exit in signal context
            break;
        case SIGCHLD:
            snprintf(msg, sizeof(msg),
                     "[%s] [DAEMON] Received SIGCHLD: (not expected in daemon, ignored).\n", ts);
            break;
        default:
            snprintf(msg, sizeof(msg),
                     "[%s] [DAEMON] Received unknown signal %d\n", ts, sig);
    }

    write(STDOUT_FILENO, msg, strlen(msg));
}

// Function to create and initialize daemon process
int daemonize() {
    pid_t pid;
    
    // Fork and let the parent exit
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        // Parent exits
        return pid;
    }
    
    // Child becomes session leader
    if (setsid() < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    
    // Fork again to ensure the daemon can't reacquire a terminal
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        // Let the parent exit
        exit(EXIT_SUCCESS);
    }
    
    // Change working directory to root
    chdir("/");
    
    // Close all open file descriptors
    for (int i = 0; i < sysconf(_SC_OPEN_MAX); i++) {
        close(i);
    }
    
    // Redirect standard file descriptors to log file
    int fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        exit(EXIT_FAILURE);
    }
    
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);

    signal(SIGUSR1, handle_daemon_signal);
    signal(SIGHUP,  handle_daemon_signal);
    signal(SIGTERM, handle_daemon_signal);
    signal(SIGCHLD, handle_daemon_signal);
    
    // Log daemon startup
    time_t now = time(NULL);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%c\n", localtime(&now));
    
    write_str(STDOUT_FILENO, "Daemon started at ");
    write_str(STDOUT_FILENO, time_str);
    write_str(STDOUT_FILENO, "Daemon PID: ");
    write_int(STDOUT_FILENO, getpid());
    write_str(STDOUT_FILENO, "\n");
    
    return 0;
}

// Function to handle timeouts for child processes
void monitor_children(pid_t child1_pid, pid_t child2_pid) {
    // Set a timeout value (e.g., 15 seconds)
    int timeout = 15;
    int elapsed = 0;
    
    while (elapsed < timeout) {
        sleep(1);
        elapsed++;
        
        // Check if children are still running
        if (kill(child1_pid, 0) < 0 && errno == ESRCH && 
            kill(child2_pid, 0) < 0 && errno == ESRCH) {
            // Both children have exited
            break;
        }
    }
    
    // If timeout reached, terminate any remaining children
    if (elapsed >= timeout) {
        write_str(STDOUT_FILENO, "Timeout reached. Terminating any lingering child processes.\n");
        if (kill(child1_pid, 0) == 0) {
            kill(child1_pid, SIGTERM);
            write_str(STDOUT_FILENO, "Terminated unresponsive child process ");
            write_int(STDOUT_FILENO, child1_pid);
            write_str(STDOUT_FILENO, "\n");
        }
        if (kill(child2_pid, 0) == 0) {
            kill(child2_pid, SIGTERM);
            write_str(STDOUT_FILENO, "Terminated unresponsive child process ");
            write_int(STDOUT_FILENO, child2_pid);
            write_str(STDOUT_FILENO, "\n");
        }
    }
}

int main(int argc, char *argv[]) {
    
    // Check for correct number of arguments
    if (argc != 3) {
        write_str(STDERR_FILENO, "Usage: ");
        write_str(STDERR_FILENO, argv[0]);
        write_str(STDERR_FILENO, " <integer1> <integer2>\n");
        exit(EXIT_FAILURE);
    }
    
    // Convert command line arguments to integers
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    
    write_str(STDOUT_FILENO, "Received integers: ");
    write_int(STDOUT_FILENO, num1);
    write_str(STDOUT_FILENO, " and ");
    write_int(STDOUT_FILENO, num2);
    write_str(STDOUT_FILENO, "\n");
    
    // Create FIFOs if they don't exist
    if (mkfifo(FIFO1, 0666) < 0 && errno != EEXIST) {
        perror("mkfifo FIFO1");
        exit(EXIT_FAILURE);
    }
    
    if (mkfifo(FIFO2, 0666) < 0 && errno != EEXIST) {
        perror("mkfifo FIFO2");
        exit(EXIT_FAILURE);
    }

    if (mkfifo(DAEMON_FILE, 0666) < 0 && errno != EEXIST) {
        perror("mkfifo DAEMONFILE");
        exit(EXIT_FAILURE);
    }

    // Create first child process
    pid_t child1_pid = fork();
    
    char creation_time1[32];
    time_t now = time(NULL);
    strftime(creation_time1, sizeof(creation_time1), "%c\n", localtime(&now));
    
    if (child1_pid < 0) {
        perror("fork child1");
        exit(EXIT_FAILURE);
    } else if (child1_pid == 0) {
        // This is Child Process 1
        sleep(10); // Sleep for 10 seconds as required
        
        // Open FIFO1 for reading
        int fd1 = open(FIFO1, O_RDONLY);
        if (fd1 < 0) {
            perror("open FIFO1");
            exit(EXIT_FAILURE);
        }
        
        // Read the two integers
        int values[2];
        if (read(fd1, values, sizeof(values)) != sizeof(values)) {
            perror("read FIFO1");
            close(fd1);
            exit(EXIT_FAILURE);
        }
        close(fd1);
        
        // Determine the larger number
        int larger = (values[0] > values[1]) ? values[0] : values[1];
        write_str(STDOUT_FILENO, "Child 1: Larger number is ");
        write_int(STDOUT_FILENO, larger);
        write_str(STDOUT_FILENO, "\n");
        
        // Open FIFO2 for writing
        int fd2 = open(FIFO2, O_WRONLY);
        if (fd2 < 0) {
            perror("open FIFO2");
            exit(EXIT_FAILURE);
        }
        
        // Write the larger number to FIFO2
        if (write(fd2, &larger, sizeof(larger)) != sizeof(larger)) {
            perror("write FIFO2");
            close(fd2);
            exit(EXIT_FAILURE);
        }
        close(fd2);
        
        write_str(STDOUT_FILENO, "Child 1: Sent larger number to FIFO2\n");
        exit(EXIT_SUCCESS); //First child exits
    }
    
    // Create second child process
    pid_t child2_pid = fork();
    
    char creation_time2[32];
    now = time(NULL);
    strftime(creation_time2, sizeof(creation_time2), "%c\n", localtime(&now));

    if (child2_pid < 0) {
        perror("fork child2");
        exit(EXIT_FAILURE);
    } else if (child2_pid == 0) {
        // This is Child Process 2
        sleep(10); // Sleep for 10 seconds as required
        
        // Open FIFO2 for reading
        int fd2 = open(FIFO2, O_RDONLY);
        if (fd2 < 0) {
            perror("open FIFO2");
            exit(EXIT_FAILURE);
        }
        
        // Read the larger number
        int larger;
        if (read(fd2, &larger, sizeof(larger)) != sizeof(larger)) {
            perror("read FIFO2");
            close(fd2);
            exit(EXIT_FAILURE);
        }
        close(fd2);
        
        // Print the larger number
        write_str(STDOUT_FILENO, "Child 2: The larger number is: ");
        write_int(STDOUT_FILENO, larger);
        write_str(STDOUT_FILENO, "\n");
        exit(EXIT_SUCCESS); //Second child exits
    }
    
    // Parent process
    write_str(STDOUT_FILENO, "Parent: Created child processes with PIDs ");
    write_int(STDOUT_FILENO, child1_pid);
    write_str(STDOUT_FILENO, " and ");
    write_int(STDOUT_FILENO, child2_pid);
    write_str(STDOUT_FILENO, "\n");
    
    // Create daemon process
    pid_t daemon_pid = daemonize();
    if (daemon_pid > 0) {
        write_str(STDOUT_FILENO, "Parent: Created daemon process.\n");
        
        // Open FIFO1 for writing with O_NONBLOCK
        int fd1;
        while ((fd1 = open(FIFO1, O_WRONLY | O_NONBLOCK)) < 0) {
            if (errno == ENXIO) {
                write_str(STDOUT_FILENO, "Parent: proceeding...\n");
                sleep(2);
            } else {
                perror("open FIFO1");
                exit(EXIT_FAILURE);
            }
        }
        
        // Write the two integers to FIFO1
        int values[2] = {num1, num2};
        if (write(fd1, values, sizeof(values)) != sizeof(values)) {
            perror("write FIFO1");
            close(fd1);
            exit(EXIT_FAILURE);
        }
        close(fd1);

        write_str(STDOUT_FILENO, "Parent: Sent integers to FIFO1\n");
        
        // Set up signal handler for SIGCHLD
        if (signal(SIGCHLD, handle_sigchld) == SIG_ERR) {
            perror("signal");
            exit(EXIT_FAILURE);
        }
        
        // Loop, printing a message every two seconds
        while (child_counter < (num_children * 2)) {
            sleep(2);
        }
        
        write_str(STDOUT_FILENO, "Parent: All child processes have terminated.\n");
        exit(EXIT_SUCCESS);
    } else {
        // This is the daemon process

        //Logging child processes
        write_str(STDOUT_FILENO, "Created child processes with PIDs ");
        write_int(STDOUT_FILENO, child1_pid);
        write_str(STDOUT_FILENO, " at ");
        write_str(STDOUT_FILENO, creation_time1);

        write_str(STDOUT_FILENO, "Created child processes with PIDs ");
        write_int(STDOUT_FILENO, child2_pid);
        write_str(STDOUT_FILENO, " at ");
        write_str(STDOUT_FILENO, creation_time2);

        int fd1 = open(DAEMON_FILE, O_RDONLY);
        if (fd1 < 0) {
            perror("open DAEMON_FILE");
            exit(EXIT_FAILURE);
        }

        char buffer[256];
        while (1) {
            ssize_t len = read(fd1, buffer, sizeof(buffer) - 1);
            if (len > 0) {
                buffer[len] = '\0';  // Null-terminate
                write_str(STDOUT_FILENO, buffer);  // Log to log file
            } else {  // Daemon continues running and handling signals
                monitor_children(child1_pid, child2_pid);
                sleep(1);  // No data, prevent busy loop
            }
        }

    }
    
    return 0;
}