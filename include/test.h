#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <time.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>

// ANSI color codes for pretty terminal output
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_RESET "\033[0m"

// Global counters for stats
extern int total_tests;
extern int passed_tests;
extern int failed_tests;

// Global log file pointer
extern FILE *log_file;

// Function to log a header with a timestamp
void log_header(const char *title)
{
    time_t now = time(NULL);
    char time_buffer[30];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(log_file, "===============================\n");
    fprintf(log_file, "%s\n", title);
    fprintf(log_file, "Timestamp: %s\n", time_buffer);
    fprintf(log_file, "===============================\n\n");
}

// Function to log a test result
void log_test_result(const char *test_name, int passed)
{
    total_tests++;

    time_t now = time(NULL);
    char time_buffer[30];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(log_file, "Test: %s\n", test_name);
    fprintf(log_file, "Timestamp: %s\n", time_buffer);
    if (passed)
    {
        passed_tests++;
        fprintf(log_file, "[PASS] %s\n", test_name);
        printf(COLOR_GREEN "[PASS] %s\n" COLOR_RESET, test_name);
    }
    else
    {
        failed_tests++;
        fprintf(log_file, "[FAIL] %s\n", test_name);
        printf(COLOR_RED "[FAIL] %s\n" COLOR_RESET, test_name);
    }
    fprintf(log_file, "---------------------------------\n");
}

// Function to log the test summary
void log_test_summary(clock_t start_time)
{
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    fprintf(log_file, "\n===============================\n");
    fprintf(log_file, "Run Summary:\n");
    fprintf(log_file, "    Type       Total    Ran Passed Failed\n");
    fprintf(log_file, "    tests      %-8d%-8d%-8d%-8d\n", total_tests, total_tests, passed_tests, failed_tests);
    fprintf(log_file, "Elapsed time = %.3f seconds\n", elapsed_time);
    fprintf(log_file, "===============================\n");
}

#endif // TEST_UTILS_H
