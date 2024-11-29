# Compiler
CC = gcc

# Development flags (with testing)
DEV_CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
DEV_LDFLAGS = -pthread -lcunit -lm

# Production flags
PROD_CFLAGS = -Wall -Wextra -O2
PROD_LDFLAGS = -pthread

# Common sanitizer flags (updated to separate incompatible flags)
SANITIZER_FLAGS = -fsanitize=address     # Memory errors (buffer overflows, use-after-free, etc.)
SANITIZER_FLAGS += -fsanitize=undefined  # Undefined behavior
SANITIZER_FLAGS += -fsanitize=leak      # Memory leaks
# Remove thread sanitizer from default flags since it's incompatible with address/leak sanitizer

# Debug flags
DEBUG_FLAGS = -g -fno-omit-frame-pointer  # Better debug info and stack traces

# Warning flags
WARN_FLAGS = -Wall -Wextra -Werror -Wformat=2 -Wformat-security

CFLAGS = $(WARN_FLAGS) $(SANITIZER_FLAGS) $(DEBUG_FLAGS)
LDFLAGS = $(DEV_LDFLAGS)

# Use production flags if PROD is set
ifeq ($(PROD),1)
    CFLAGS = $(PROD_CFLAGS)
    LDFLAGS = $(PROD_LDFLAGS)
endif

INCLUDE = -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
TEST_DIR = test
BIN_DIR = bin

# Source files for main program
MAIN_SRC = $(SRC_DIR)/main.c
COMMON_SRC = $(filter-out $(MAIN_SRC), $(wildcard $(SRC_DIR)/*.c))
COMMON_OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/src/%.o, $(COMMON_SRC))
MAIN_OBJ = $(COMMON_OBJ) $(OBJ_DIR)/src/main.o

# Test files and objects
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/test/%.o, $(TEST_SRC))
TEST_EXEC = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(TEST_SRC))

# Test utilities
TEST_UTILS_SRC = $(TEST_DIR)/test_utils.c
TEST_UTILS_OBJ = $(OBJ_DIR)/test/test_utils.o

# Add LOG_DIR if not already defined
LOG_DIR = log

# Create log directory if it doesn't exist
$(LOG_DIR):
	@mkdir -p $(LOG_DIR)

# Default rule
all: $(BIN_DIR)/main

# Build the main executable
$(BIN_DIR)/main: $(MAIN_OBJ) | $(BIN_DIR)
	@echo "Linking main..."
	$(CC) $(CFLAGS) $(INCLUDE) $(MAIN_OBJ) $(LDFLAGS) -o $@

# Build test objects
$(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)/test
	@echo "Compiling test $< into $@..."
	$(CC) $(CFLAGS) -DTESTING $(INCLUDE) -c $< -o $@

# Build source objects
$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/src
	@echo "Compiling $< into $@..."
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Build individual test executables
$(BIN_DIR)/%: $(OBJ_DIR)/test/%.o $(COMMON_OBJ) $(TEST_UTILS_OBJ) | $(BIN_DIR)
	@echo "Linking test $@..."
	$(CC) $(CFLAGS) -DTESTING $(INCLUDE) $^ $(LDFLAGS) -o $@

# Create directories
$(OBJ_DIR)/src:
	@mkdir -p $(OBJ_DIR)/src

$(OBJ_DIR)/test:
	@mkdir -p $(OBJ_DIR)/test

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# User-friendly targets
.PHONY: run test-jobs test-execution help prod clean

run: $(BIN_DIR)/main | $(LOG_DIR)
	@echo "\n=== Running Main Program (Job Processing System) ===\n"
	./bin/main

test-jobs: $(BIN_DIR)/test_job_generation
	@echo "\n=== Running Job Generation Tests ===\n"
	./bin/test_job_generation

test-execution: $(BIN_DIR)/test_execution
	@echo "\n=== Running Job Execution Tests ===\n"
	./bin/test_execution

test-scheduler: $(BIN_DIR)/test_scheduler
	@echo "\n=== Running Job Scheduler Tests ===\n"
	./bin/test_scheduler

$(BIN_DIR)/test_scheduler: $(OBJ_DIR)/test/test_schedule.o $(COMMON_OBJ) $(TEST_UTILS_OBJ) | $(BIN_DIR)
	@echo "Linking test scheduler..."
	$(CC) $(CFLAGS) -DTESTING $(INCLUDE) $^ $(LDFLAGS) -o $@

prod:
	@$(MAKE) clean
	@$(MAKE) PROD=1 run

help:
	@echo "\nAvailable commands:"
	@echo "  make run            - Run the main job processing system"
	@echo "  make test-jobs      - Run job generation tests"
	@echo "  make test-execution - Run job execution tests"
	@echo "  make test-scheduler - Run job scheduler tests"
	@echo "  make prod          - Build and run production version"
	@echo "  make clean         - Clean all built files"
	@echo "  make help          - Show this help message\n"

# Clean
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

clean-logs:
	@echo "Cleaning all logs..."
	rm -rf $(LOG_DIR)/*

clean-all: clean clean-logs

.PHONY: all clean tests run-test-%

# For memory testing (removed thread sanitizer)
test-memory: CFLAGS += -fsanitize=address,undefined,leak
test-memory: LDFLAGS += -lasan -lubsan
test-memory: $(OBJ_DIR)/test/test_schedule.o $(COMMON_OBJ) $(TEST_UTILS_OBJ) | $(BIN_DIR)
	@echo "Linking test scheduler with memory sanitizers..."
	$(CC) $(CFLAGS) -DTESTING $(INCLUDE) $^ $(LDFLAGS) -o $(BIN_DIR)/test_scheduler
	./bin/test_scheduler

# For thread testing (separate target with only thread sanitizer)
test-threads: CFLAGS = $(WARN_FLAGS) $(DEBUG_FLAGS)
test-threads: LDFLAGS = -pthread -lcunit -lm
test-threads: clean
	@mkdir -p $(OBJ_DIR)/src $(OBJ_DIR)/test $(BIN_DIR)
	$(CC) $(CFLAGS) -DTESTING $(INCLUDE) -c $(TEST_DIR)/test_schedule.c -o $(OBJ_DIR)/test/test_schedule.o
	$(CC) $(CFLAGS) -DTESTING $(INCLUDE) -c $(TEST_DIR)/test_utils.c -o $(OBJ_DIR)/test/test_utils.o
	for src in $(COMMON_SRC); do \
		$(CC) $(CFLAGS) $(INCLUDE) -c $$src -o $(OBJ_DIR)/src/$$(basename $$src .c).o; \
	done
	$(CC) $(CFLAGS) -DTESTING $(INCLUDE) $(OBJ_DIR)/test/test_schedule.o $(COMMON_OBJ) $(OBJ_DIR)/test/test_utils.o $(LDFLAGS) -o $(BIN_DIR)/test_scheduler
	@echo "=== Running tests with Helgrind ==="
	valgrind --tool=helgrind ./bin/test_scheduler

test-threads-stress: test-threads
	for i in {1..100}; do \
		TSAN_OPTIONS="halt_on_error=1" \
		./bin/test_scheduler || break; \
		echo "Pass $$i completed successfully"; \
	done

.PHONY: test-threads

test-threads-valgrind:
	$(MAKE) CFLAGS="$(WARN_FLAGS) $(DEBUG_FLAGS)" LDFLAGS="-pthread -lcunit -lm" all
	for i in {1..5}; do \
		valgrind --tool=helgrind ./bin/test_scheduler || exit 1; \
	done
