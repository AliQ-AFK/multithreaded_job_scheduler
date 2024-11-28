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
SRC = $(wildcard $(SRC_DIR)/*.c)  # All source files in src
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/src/%.o, $(SRC))  # Object files for src

# Test files
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_EXEC = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(TEST_SRC))  # Test executables

# Target executables
MAIN_EXEC = main
TEST_EXEC_NAMES = test_execution test_job_generation  # Add more as needed

# Default rule
all: $(MAIN_EXEC)

# Build the main executable
$(MAIN_EXEC): $(OBJ) | $(BIN_DIR)
	@echo "Linking $(MAIN_EXEC)..."
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) $(LDFLAGS) -o $(BIN_DIR)/$(MAIN_EXEC)

# Build individual test executables
$(BIN_DIR)/%: $(TEST_DIR)/%.c $(OBJ) | $(BIN_DIR)
	@echo "Compiling $< into $@..."
	$(CC) $(CFLAGS) $(INCLUDE) $< $(OBJ) $(LDFLAGS) -o $@

# Build object files for src
$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/src
	@echo "Compiling $< into $@..."
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR)/src:
	@mkdir -p $(OBJ_DIR)/src

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Target to build all tests
tests: $(TEST_EXEC)

# Run specific test
run-test-%: $(BIN_DIR)/%
	@echo "Running $<..."
	./$<

# Clean up build files
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.log $(MAIN_EXEC)

# Phony targets
.PHONY: all clean tests run-test-%
