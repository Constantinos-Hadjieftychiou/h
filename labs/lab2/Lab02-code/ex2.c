#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// Function to calculate Fibonacci number
int fibonacci(int n)
{
    if (n <= 2)
    {
        return 1;
    }
    else
    {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

// Function to print Fibonacci numbers in a child process
void printFibonacci(int n)
{
    for (int i = 1; i <= n; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            // Child process
            int fibNumber = fibonacci(i);
            printf("Child Process %d: Fibonacci(%d) = %d\n", getpid(), i, fibNumber);
            exit(0); // Child process exits after printing the Fibonacci number
        }
        else if (pid < 0)
        {
            // Forking error
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Parent process
            wait(NULL); // Wait for the child process to finish
        }
    }
}

int main()
{
    int numFibonacci;

    // Take the number of Fibonacci numbers to be printed as input
    printf("Enter the number of Fibonacci numbers to be printed: ");
    scanf("%d", &numFibonacci);

    // Call the function to print Fibonacci numbers in child processes
    printFibonacci(numFibonacci);

    return 0;
}
