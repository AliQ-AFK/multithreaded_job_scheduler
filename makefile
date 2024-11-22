# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=address -g
INCLUDE = -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Source and object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Target executable
TARGET = test

# Default rule
all: $(OBJ_DIR) $(TARGET)

# Build the executable
$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^

# Build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Create object directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Clean up build files
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(TARGET) *.log

# Phony targets
.PHONY: all clean
