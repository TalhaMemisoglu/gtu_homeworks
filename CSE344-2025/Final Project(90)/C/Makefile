CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
LDFLAGS = -pthread

all: chatserver chatclient

chatserver: chatserver.o
	$(CC) $(CFLAGS) -o chatserver chatserver.o $(LDFLAGS)

chatclient: chatclient.o
	$(CC) $(CFLAGS) -o chatclient chatclient.o $(LDFLAGS)

chatserver.o: chatserver.c
	$(CC) $(CFLAGS) -c chatserver.c

client.o: chatclient.c
	$(CC) $(CFLAGS) -c chatclient.c

clean:
	rm -f chatserver chatclient *.o

.PHONY: all clean
