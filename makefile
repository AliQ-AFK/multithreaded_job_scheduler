# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=address -g
INCLUDE = -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = test
LOGS = *.log

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

# Clean generated files
clean:
	rm -f $(OBJ) $(TARGET) $(LOGS)

# Phony targets
.PHONY: all clean
