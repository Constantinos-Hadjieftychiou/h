#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global sum variable
int global_sum = 0;
pthread_mutex_t lock;

// Struct to hold thread arguments
typedef struct
{
    int *array;
    int start;
    int end;
} ThreadArgs;

// Function to sum a portion of the array
void *sum_array_partial(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int partial_sum = 0;
    for (int i = args->start; i < args->end; i++)
    {
        partial_sum += args->array[i];
    }
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

    int array_size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    if (array_size % num_threads != 0)
    {
        printf("Array size must be perfectly divisible by the number of threads.\n");
        return 1;
    }

    // Create the array
    int *arr = (int *)malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++)
    {
        arr[i] = i + 1;
    }

    // Initialize mutex
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Mutex initialization failed.\n");
        return 1;
    }

    // Divide the array into chunks for each thread
    int chunk_size = array_size / num_threads;

    // Create thread handles
    pthread_t threads[num_threads];

    // Create and start threads
    for (int i = 0; i < num_threads; i++)
    {
        ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        args->array = arr;
        args->start = i * chunk_size;
        args->end = (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, sum_array_partial, (void *)args);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Output the global sum
    printf("Global Sum: %d\n", global_sum);

    // Clean up
    free(arr);
    pthread_mutex_destroy(&lock);

    return 0;
}
