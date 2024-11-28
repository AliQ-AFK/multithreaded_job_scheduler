# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
LDFLAGS = -pthread -lcunit -lm

INCLUDE = -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
TEST_DIR = test
BIN_DIR = bin

# Source and object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Test files
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_EXEC = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(TEST_SRC))

# Target executable
TARGET = main

# Default rule
all: $(OBJ_DIR) $(BIN_DIR) $(TARGET) $(TEST_EXEC)

# Build the main executable
$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) $(LDFLAGS) -o $@

# Build test executables
$(BIN_DIR)/%: $(TEST_DIR)/%.c $(OBJ)
	@echo "Compiling $< into $@..."
	$(CC) $(CFLAGS) $(INCLUDE) $< $(OBJ) $(LDFLAGS) -o $@

# Build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $< into $@..."
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Clean up build files
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(TARGET) *.log

# Run tests
run-tests: $(TEST_EXEC)
	@echo "Running tests..."
	@for test_exec in $(TEST_EXEC); do \
		echo "Running $$test_exec..."; \
		./$$test_exec; \
	done

# Phony targets
.PHONY: all clean run-tests
