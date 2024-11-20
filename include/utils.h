#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

// Project headers
#include "job.h"

// Common defines
#define TIME_SCALE 100000  // 0.1 seconds = 100000 microseconds
#define TIME_SLICE 2
#define MAX_TIME 100

// ANSI Color codes
/*#define ANSI_COLOR_RED     "\033[0;31m"
#define ANSI_COLOR_GREEN   "\033[0;32m"
#define ANSI_COLOR_YELLOW  "\033[0;33m"
#define ANSI_COLOR_BLUE    "\033[0;34m"
define ANSI_COLOR_RESET   "\033[0m"*/

// Error messages
#define ERR_MEMORY "Failed to allocate memory\n"
#define ERR_INPUT "Invalid input\n"


int find_next_job(job* jobs, int num_jobs, const char* type, int elapsed_time);
void schedule_jobs(job* jobs, int num_jobs); 

#endif 