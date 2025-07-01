CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = bankServer

all: clean $(TARGET) run 

$(TARGET): bankServer.c
	$(CC) $(CFLAGS) -o $(TARGET) bankServer.c
	$(CC) $(CFLAGS) -o bankClient bankClient.c

run:
	./bankServer AdaBank ServerFIFO_Adabank0178

clean:
	rm -f $(TARGET)
	rm -f bankClient