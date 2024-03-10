#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 1000000

// Global variables
int array[MAX_SIZE];
int array_size;
int num_threads;
int global_sum = 0;
pthread_mutex_t lock;

// Function to sum up a portion of the array
void *sum_array_partial(void *arg)
{
    int thread_id = *(int *)arg;
    int start = thread_id * (array_size / num_threads);
    int end = (thread_id + 1) * (array_size / num_threads);

    int partial_sum = 0;
    for (int i = start; i < end; ++i)
    {
        partial_sum += array[i];
    }

    // Locking and updating global sum
    pthread_mutex_lock(&lock);
    global_sum += partial_sum;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s array_size num_threads\n", argv[0]);
        return 1;
    }

    array_size = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    if (array_size % num_threads != 0)
    {
        printf("Array size must be perfectly divisible by the number of threads.\n");
        return 1;
    }

    // Initialize the array
    for (int i = 0; i < array_size; ++i)
    {
        array[i] = i + 1;
    }

    // Initialize mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex initialization failed.\n");
        return 1;
    }

    // Create threads
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    for (int i = 0; i < num_threads; ++i)
    {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, sum_array_partial, (void *)&thread_ids[i]) != 0)
        {
            printf("Thread creation failed.\n");
            return 1;
        }
    }

    // Join threads
    for (int i = 0; i < num_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy mutex
    pthread_mutex_destroy(&lock);

    // Output the global sum
    printf("Global Sum: %d\n", global_sum);

    return 0;
}
