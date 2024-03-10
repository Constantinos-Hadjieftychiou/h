#include <stdio.h>
#include <pthread.h>

#define MAX_SIZE  5
pthread_mutex_t bufLock;
pthread_cond_t notFull, notEmpty;
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
	int i=0;
	for(;;) {
		pthread_mutex_lock(&bufLock);
		while(count == MAX_SIZE)
			pthread_cond_wait(&notFull, &bufLock); 
		buf[count] = getChar();
		count++;
		pthread_cond_signal(&notEmpty);
		pthread_mutex_unlock(&bufLock);
		// Added a random wait to show thread behaviour 
		if (++i%10==0)
			sleep((rand() % 3) + 5);
		else
			usleep(((rand() % 10) + 1) * 100000);
	}
}	

void consumer(char* buf) {
	int i=0;
	for(;;) {
		pthread_mutex_lock(&bufLock);
		while(count == 0)
			pthread_cond_wait(&notEmpty, &bufLock);
		useChar(buf[count-1]);
		count--;
		pthread_cond_signal(&notFull);
		pthread_mutex_unlock(&bufLock);
		// Added a random wait to show thread behaviour 
		if (++i%10==0)
			sleep((rand() % 3) + 1);
		else
			usleep(((rand() % 16) + 5) * 100000);
	}
}

int main() {
	char buf[MAX_SIZE];
	pthread_t p;
	count = 0;
	pthread_mutex_init(&bufLock, NULL);
	pthread_cond_init(&notFull, NULL);
	pthread_cond_init(&notEmpty, NULL);
	pthread_create(&p, NULL, (void*)producer, &buf);  
	consumer(buf);
	return 0;
}
