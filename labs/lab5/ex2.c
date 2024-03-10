#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 10

// Global variables
pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;
int count = 0;
int threads_done = 0;
int limit_reached = 0;
int limit;

// Function to increment count
void *increment_count(void *arg)
{
    int thread_id = *((int *)arg);
    int tcount = *((int *)arg + 1);
    for (int i = 0; i < tcount; i++)
    {
        pthread_mutex_lock(&count_lock);
        count++;
        printf("Thread %d incremented count to %d\n", thread_id, count);
        if (count >= limit)
        {
            limit_reached = 1;
            printf("Limit reached. Notifying watcher thread.\n");
            count += 1000; // Add 1000 to count
            printf("Count modified to %d\n", count);
        }
        else
        {
            printf("Count is %d, limit is %d\n", count, limit);
        }
        pthread_mutex_unlock(&count_lock);
    }
    pthread_exit(NULL);
}

// Function for watcher thread
void *watcher(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&count_lock);
        if (limit_reached)
        {
            threads_done++;
            printf("Watcher notified. Threads done: %d\n", threads_done);
            if (threads_done == *((int *)arg))
            {
                pthread_mutex_unlock(&count_lock);
                break;
            }
        }
        pthread_mutex_unlock(&count_lock);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s num_threads tcount\n", argv[0]);
        exit(1);
    }

    int num_threads = atoi(argv[1]);
    int tcount = atoi(argv[2]);
    limit = (int)(num_threads * tcount * 0.6);

    pthread_t threads[MAX_THREADS];
    pthread_t watcher_thread;

    int thread_args[MAX_THREADS][2];
    for (int i = 0; i < num_threads; i++)
    {
        thread_args[i][0] = i;
        thread_args[i][1] = tcount;
        pthread_create(&threads[i], NULL, increment_count, (void *)&thread_args[i]);
    }

    pthread_create(&watcher_thread, NULL, watcher, (void *)&num_threads);

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_join(watcher_thread, NULL);

    printf("Final count: %d\n", count);

    return 0;
}
