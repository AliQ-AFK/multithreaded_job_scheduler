#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include "job.h"

// Common defines
#define TIME_SCALE 100000  // 0.1 seconds = 100000 microseconds
#define TIME_SLICE 2
#define MAX_TIME 100
#define LOG_DIR "log"

int find_next_job(job* jobs, int* num_jobs, const char* type, int elapsed_time);
FILE* open_log_file(const char* filename, const char* mode);

#endif 