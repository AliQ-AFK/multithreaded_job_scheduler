#include "test_utils.h"

void log_header(FILE *log_file, const char *title) {
    time_t now = time(NULL);
    char time_buffer[30];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(log_file, "===============================\n");
    fprintf(log_file, "%s\n", title);
    fprintf(log_file, "Timestamp: %s\n", time_buffer);
    fprintf(log_file, "===============================\n\n");
}

void log_test_result(FILE *log_file, TestStats *stats, const char *test_name, int passed) {
    stats->total_tests++;

    time_t now = time(NULL);
    char time_buffer[30];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(log_file, "Test: %s\n", test_name);
    fprintf(log_file, "Timestamp: %s\n", time_buffer);
    if (passed) {
        stats->passed_tests++;
        fprintf(log_file, "[PASS] %s\n", test_name);
        printf(COLOR_GREEN "[PASS] %s\n" COLOR_RESET, test_name);
    } else {
        stats->failed_tests++;
        fprintf(log_file, "[FAIL] %s\n", test_name);
        printf(COLOR_RED "[FAIL] %s\n" COLOR_RESET, test_name);
    }
    fprintf(log_file, "---------------------------------\n");
}

void log_test_summary(FILE *log_file, TestStats *stats, clock_t start_time) {
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    fprintf(log_file, "\n===============================\n");
    fprintf(log_file, "Run Summary:\n");
    fprintf(log_file, "    Type       Total    Ran Passed Failed\n");
    fprintf(log_file, "    tests      %-8d%-8d%-8d%-8d\n", 
            stats->total_tests, stats->total_tests, stats->passed_tests, stats->failed_tests);
    fprintf(log_file, "Elapsed time = %.3f seconds\n", elapsed_time);
    fprintf(log_file, "===============================\n");
}
