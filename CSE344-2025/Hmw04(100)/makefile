# CSE 344 - Homework #4
# Makefile for Multithreaded Log File Analyzer

CC = gcc
CFLAGS = -Wall -Wextra -g -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lpthread

TARGET = LogAnalyzer
SOURCES = 210104004009_main.c buffer.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(TARGET) $(OBJECTS)

# Dependencies
main.o: main.c buffer.h
buffer.o: buffer.c buffer.h

.PHONY: all clean