# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread
INCLUDES = -Iinclude

# Source files for each version
COMMON = src/main.c src/job_generation.c src/schedule.c src/utils.c

# Build and run all versions
all: clean
	@echo "\nBuilding and running all versions..."
	@make run_basic
	@echo "\n----------------------------------------"
	@make run_unsync
	@echo "\n----------------------------------------"
	@make run_mutex
	@echo "\n----------------------------------------"
	@make run_semaphore

# Individual version builds and runs
run_basic: printer_basic
	@echo "\nRunning basic version:"
	./printer_basic

run_unsync: printer_unsync
	@echo "\nRunning unsynchronized version:"
	./printer_unsync

run_mutex: printer_mutex
	@echo "\nRunning mutex version:"
	./printer_mutex

run_semaphore: printer_semaphore
	@echo "\nRunning semaphore version:"
	./printer_semaphore

# Compilation targets
printer_unsync: 
	$(CC) $(CFLAGS) $(INCLUDES) -DUNSYNC $(COMMON) src/job_execution_unsync.c -o printer_unsync

printer_mutex: 
	$(CC) $(CFLAGS) $(INCLUDES) -DMUTEX $(COMMON) src/job_execution_mutex.c -o printer_mutex

printer_semaphore: 
	$(CC) $(CFLAGS) $(INCLUDES) -DSEMAPHORE $(COMMON) src/job_execution_semaphore.c -o printer_semaphore

printer_basic: 
	$(CC) $(CFLAGS) $(INCLUDES) $(COMMON) -o printer_basic

# Clean all executables
clean:
	rm -f printer_unsync printer_mutex printer_semaphore printer_basic jobs.dat

.PHONY: all clean run_basic run_unsync run_mutex run_semaphore