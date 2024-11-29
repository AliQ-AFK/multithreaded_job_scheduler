#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>

// ANSI color codes for pretty terminal output
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_RESET "\033[0m"

//struct for detaileed logging
typedef struct
{
    int total_tests;
    int passed_tests;
    int failed_tests;
} TestStats;

// Function prototypes
void log_header(FILE *log_file, const char *title);
void log_test_result(FILE *log_file, TestStats *stats, const char *test_name, int passed);
void log_test_summary(FILE *log_file, TestStats *stats, clock_t start_time);


#endif // TEST_UTILS_H
