CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = ipc_daemon

all: $(TARGET)

$(TARGET): ipc_daemon.c
	$(CC) $(CFLAGS) -o $(TARGET) ipc_daemon.c

clean:
	rm -f $(TARGET)
	rm -f /tmp/fifo1 /tmp/fifo2 /tmp/daemon.log