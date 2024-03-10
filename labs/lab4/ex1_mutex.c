#include <stdio.h>
#include <pthread.h>

#define MAX_SIZE  5
pthread_mutex_t bufLock;
int count;

char nextChar = 'A';

char getChar() {
	char c = nextChar;
	nextChar = (nextChar=='z') ? 'A' : nextChar+1;
	printf("Producer created char %c\n", c);
	return c;
}

void useChar(char c) {
	printf("Consumer used char %c\n", c);
}


void producer(char* buf) {
	int i = 0;
	for(;;) {
		while(count == MAX_SIZE);
		pthread_mutex_lock(&bufLock);
		if(count<MAX_SIZE){
			buf[count] = getChar();
			count++;
		}
		pthread_mutex_unlock (&bufLock);
		// Added a random wait to show thread behaviour 
		if (++i%10==0)
			sleep((rand() % 3) + 5);
		else
			usleep(((rand() % 10) + 1) * 100000);
	}
}
void consumer(char* buf) {
	int i = 0;
	for(;;) {
		while(count == 0);
		pthread_mutex_lock(&bufLock);
		if(count>0){
			useChar(buf[count-1]);
			count--;
		}
		pthread_mutex_unlock(&bufLock);
		// Added a random wait to show thread behaviour 
		if (++i%10==0)
			sleep((rand() % 3) + 1);
		else
			usleep(((rand() % 16) + 5) * 100000);
	}
}

int main() {
	char buffer[MAX_SIZE];
	pthread_t p;
	count = 0;
	srand(time(NULL));
	
	pthread_mutex_init(&bufLock, NULL);
	pthread_create(&p, NULL, (void*)producer, &buffer);  
	consumer(buffer);
	return 0;
}
