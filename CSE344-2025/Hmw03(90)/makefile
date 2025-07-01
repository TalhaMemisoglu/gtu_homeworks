CC = gcc
CFLAGS = -Wall -pthread

all: clean satellite_system run

satellite_system: satellite_system.c
	$(CC) $(CFLAGS) -fsanitize=thread -g -o satellite_system satellite_system.c -pthread

run:
	./satellite_system

clean:
	rm -f satellite_system