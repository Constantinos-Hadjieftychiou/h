#include <stdio.h>
#include <semaphore.h>

#define MAX_SIZE  5
sem_t empty, full;

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
	int in = 0, i=0;
	for(;;) {
		sem_wait(&empty); 
		buf[in] = getChar();
		in = (in + 1) % MAX_SIZE;
		sem_post(&full);
		// Added a random wait to show thread behaviour 
		if (++i%10==0)
			sleep((rand() % 3) + 5);
		else
			usleep(((rand() % 10) + 1) * 100000);
	}
}	

void consumer(char* buf) {
	int out = 0, i=0;
	for(;;) {
		sem_wait(&full);
		useChar(buf[out]);
		out = (out + 1) % MAX_SIZE;
		sem_post(&empty);
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
	sem_init(&empty, 0, MAX_SIZE);
	sem_init(&full, 0, 0);
	pthread_create(&p, NULL, (void*)producer, &buffer);  
	consumer(buffer);
	return 0;
}

