#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void count_frequency(int arr[], int size)
{
    int freq[10] = {0};

    for (int i = 0; i < size; i++)
    {
        freq[arr[i]]++;
    }

    printf("Parent Process: Frequency of each number in the array:\n");
    for (int i = 1; i <= 9; i++)
    {
        printf("%d: %d\n", i, freq[i]);
    }
}

void sort_array(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                // Swap elements if they are in the wrong order
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    printf("1st Child Process: Sorted array: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void count_even_numbers(int arr[], int size)
{
    int even_count = 0;

    for (int i = 0; i < size; i++)
    {
        if (arr[i] % 2 == 0)
        {
            even_count++;
        }
    }

    printf("2nd Child Process: Total even number(s) in the array: %d\n", even_count);
}

void sum_even_numbers(int arr[], int size)
{
    int even_sum = 0;

    for (int i = 0; i < size; i++)
    {
        if (arr[i] % 2 == 0)
        {
            even_sum += arr[i];
        }
    }

    printf("3rd Child Process: Sum of even numbers in the array: %d\n", even_sum);
}

int main()
{
    int arr[10];

    // Generate an array of 10 numbers
    for (int i = 0; i < 10; i++)
    {
        arr[i] = rand() % 9 + 1;
    }

    // Create processes
    pid_t pid1, pid2, pid3;
    pid1 = fork();

    if (pid1 == 0)
    {
        // First child process (sorting)
        sort_array(arr, 10);
    }
    else
    {
        pid2 = fork();

        if (pid2 == 0)
        {
            // Second child process (counting even numbers)
            count_even_numbers(arr, 10);
        }
        else
        {
            pid3 = fork();

            if (pid3 == 0)
            {
                // Third child process (summing even numbers)
                sum_even_numbers(arr, 10);
            }
            else
            {
                // Parent process
                count_frequency(arr, 10);

                // Wait for all child processes to complete
                wait(NULL);
                wait(NULL);
                wait(NULL);
            }
        }
    }

    return 0;
}
