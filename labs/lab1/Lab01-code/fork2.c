#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 

void main() {
  int pid;
  pid = fork();
  printf("%d \n", pid);
}

