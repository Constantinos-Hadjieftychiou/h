// C program for passing value from 
// child process to parent process 
// #include <pthread.h> 
#include <stdio.h> 
#include <sys/types.h> 
//#include <unistd.h> 
//#include <stdlib.h> 
#include <sys/wait.h> 
#define MAX 10 

int main() { 

	int fd[2], i = 0; 
	pipe(fd); 
	pid_t pid = fork(); 

	if(pid > 0) { 
		// wait for child to finish 
		wait(NULL); 

		// no need to use the write end of pipe here so close it 
		close(fd[1]); 

		int arr[MAX]; 

		// n stores the total bytes read successfully 
		int n = read(fd[0], arr, sizeof(arr)); 

		for ( i = 0;i < n/4; i++) {
			// printing the array received from child process 
			printf("%d ", arr[i]); 
		}
	} else if( pid == 0 ) { 
		int arr[] = {1, 2, 3, 4, 5}; 

		// no need to use the read end of pipe here so close it 
		close(fd[0]); 

		write(fd[1], arr, sizeof(arr)); 
	} else { 
		perror("error\n"); //fork() 
	} 
} 
