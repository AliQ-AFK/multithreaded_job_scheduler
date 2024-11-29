
#include "execution.h"
#include "test_utils.h"
#include "utils.h"
#include <sys/stat.h>
#include <stdlib.h>

// Test: Process a single ready job
void test_process_job_with_one_ready_job(FILE *log_file, TestStats *stats)
{
    if (!log_file || !stats) 
    {
        printf("Error: NULL pointers passed to test\n");
        return;
    }

    // Create a test job
    job jobs[1] =
    {
        { .user_id = 1, .job_type = "print", .page = 5, .arrival_time = 0 }
    };
    int num_jobs = 1;
    unsigned int elapsed_time = 1;

    // Create a temporary log file for process_job
    FILE *temp_log = fopen("temp_execution.log", "w");
    if (!temp_log)
    {
        fprintf(log_file, "Failed to create temporary log file\n");
        return;
    }

    // Process the job
    process_job(jobs, &num_jobs, "print", &elapsed_time, temp_log, pthread_self(), NULL);

    // Close temporary log
    fclose(temp_log);

    // Verify results
    int passed = (num_jobs == 1 && jobs[0].page == 3 && (unsigned int)jobs[0].arrival_time == elapsed_time);

    CU_ASSERT_EQUAL(num_jobs, 1);
    CU_ASSERT_EQUAL(jobs[0].page, 3);
    CU_ASSERT_EQUAL(jobs[0].arrival_time, elapsed_time);

    log_test_result(log_file, stats, "test_process_job_with_one_ready_job", passed);
}

// Test mutex execution
void test_mutex_execution(FILE *log_file, TestStats *stats)
{
    if (!log_file || !stats) 
    {
        printf("Error: NULL pointers passed to test\n");
        return;
    }

    // Setup test data
    job jobs[3] = 
    {
        { .user_id = 1, .job_type = "print", .page = 4, .arrival_time = 0 },
        { .user_id = 2, .job_type = "scan", .page = 6, .arrival_time = 0 },
        { .user_id = 3, .job_type = "print", .page = 2, .arrival_time = 0 }
    };
    int num_jobs = 3;

    // Setup mutexes
    pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t scan_mutex = PTHREAD_MUTEX_INITIALIZER;

    // Setup execution args
    execution_args args = 
    {
        .jobs = jobs,
        .num_jobs = &num_jobs,
        .print_mutex = &print_mutex,
        .scan_mutex = &scan_mutex,
        .print_semaphore = NULL,
        .scan_semaphore = NULL
    };

    // Create temporary log file
    FILE *temp_log = fopen("temp_mutex.log", "w");
    if (!temp_log) {
        fprintf(log_file, "Failed to create temporary log file\n");
        return;
    }

    // Run mutex execution
    mutex_job_execution(&args);

    // Verify results
    int passed = (num_jobs == 0); // All jobs should be processed
    
    CU_ASSERT_EQUAL(num_jobs, 0);

    // Cleanup
    pthread_mutex_destroy(&print_mutex);
    pthread_mutex_destroy(&scan_mutex);
    fclose(temp_log);

    log_test_result(log_file, stats, "test_mutex_execution", passed);
}

// Test semaphore execution
void test_semaphore_execution(FILE *log_file, TestStats *stats)
{
    if (!log_file || !stats) 
    {
        printf("Error: NULL pointers passed to test\n");
        return;
    }

    // Setup test data
    job jobs[3] = 
    {
        { .user_id = 1, .job_type = "print", .page = 4, .arrival_time = 0 },
        { .user_id = 2, .job_type = "scan", .page = 6, .arrival_time = 0 },
        { .user_id = 3, .job_type = "print", .page = 2, .arrival_time = 0 }
    };
    int num_jobs = 3;

    // Setup semaphores
    sem_t print_semaphore, scan_semaphore;
    sem_init(&print_semaphore, 0, 1);
    sem_init(&scan_semaphore, 0, 1);

    // Setup execution args
    execution_args args = 
    {
        .jobs = jobs,
        .num_jobs = &num_jobs,
        .print_mutex = NULL,
        .scan_mutex = NULL,
        .print_semaphore = &print_semaphore,
        .scan_semaphore = &scan_semaphore
    };

    // Create temporary log file
    FILE *temp_log = fopen("temp_semaphore.log", "w");
    if (!temp_log)
    {
        fprintf(log_file, "Failed to create temporary log file\n");
        return;
    }

    // Run semaphore execution
    semaphore_job_execution(&args);

    // Verify results
    int passed = (num_jobs == 0); // All jobs should be processed
    
    CU_ASSERT_EQUAL(num_jobs, 0);

    // Cleanup
    sem_destroy(&print_semaphore);
    sem_destroy(&scan_semaphore);
    fclose(temp_log);

    log_test_result(log_file, stats, "test_semaphore_execution", passed);
}

// Test unsynchronized execution
void test_unsync_execution(FILE *log_file, TestStats *stats)
{
    if (!log_file || !stats)
    {
        printf("Error: NULL pointers passed to test\n");
        return;
    }

    // Setup test data
    job jobs[3] = 
    {
        { .user_id = 1, .job_type = "print", .page = 4, .arrival_time = 0 },
        { .user_id = 2, .job_type = "scan", .page = 6, .arrival_time = 0 },
        { .user_id = 3, .job_type = "print", .page = 2, .arrival_time = 0 }
    };
    int num_jobs = 3;

    // Setup execution args
    execution_args args =
    {
        .jobs = jobs,
        .num_jobs = &num_jobs,
        .print_mutex = NULL,
        .scan_mutex = NULL,
        .print_semaphore = NULL,
        .scan_semaphore = NULL
    };

    // Create temporary log file
    FILE *temp_log = fopen("temp_unsync.log", "w");
    if (!temp_log)
    {
        fprintf(log_file, "Failed to create temporary log file\n");
        return;
    }

    // Run unsynchronized execution
    unsynced_job_execution(&args);

    // Verify results
    int passed = (num_jobs == 0); // All jobs should be processed
    
    CU_ASSERT_EQUAL(num_jobs, 0);

    // Cleanup
    fclose(temp_log);

    log_test_result(log_file, stats, "test_unsync_execution", passed);
}

int main()
{
    // Ensure the logs directory exists
    mkdir("logs", 0777);

    // Open the log file
    FILE *log_file = fopen("logs/test_execution.log", "w");
    if (!log_file)
    {
        perror("Failed to open log file");
        return 1;
    }

    // Initialize the TestStats struct
    TestStats stats = {0};

    log_header(log_file, "Job Execution Tests");

    // Initialize the CUnit registry
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        perror("CUnit initialization failed");
        fclose(log_file);
        return 1;
    }

    CU_pSuite suite = CU_add_suite("Execution Tests", NULL, NULL);
    if (!suite)
    {
        CU_cleanup_registry();
        fclose(log_file);
        return 1;
    }

    // Add the test case
    if (NULL == CU_add_test(suite, "test_process_job_with_one_ready_job",
                (CU_TestFunc)test_process_job_with_one_ready_job))
    {
        CU_cleanup_registry();
        fclose(log_file);
        return 1;
    }

    // Add all test cases
    CU_add_test(suite, "test_mutex_execution",
                (CU_TestFunc)test_mutex_execution);
    CU_add_test(suite, "test_semaphore_execution",
                (CU_TestFunc)test_semaphore_execution);
    CU_add_test(suite, "test_unsync_execution",
                (CU_TestFunc)test_unsync_execution);

    clock_t start_time = clock();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    log_test_summary(log_file, &stats, start_time);

    CU_cleanup_registry();
    fclose(log_file);

    return 0;
}
