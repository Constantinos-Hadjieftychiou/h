#include <stdio.h>
#include <pthread.h>
#include  "ConsumerProducerMonitor.h"

#define MAX_SIZE  5

static pthread_once_t is_initialized = PTHREAD_ONCE_INIT; 
static pthread_mutex_t bufLock;
static pthread_cond_t notFull, notEmpty;


static char buf[MAX_SIZE];
static int count = 0;
static char nextChar = 'A';

static char getChar() {
	char c = nextChar;
	nextChar = (nextChar=='z') ? 'A' : nextChar+1;
	printf("Producer created char %c\n", c);
	return c;
}

static void useChar(char c) {
	printf("Consumer used char %c\n", c);
}

extern void init() {
	pthread_mutex_init(&bufLock, NULL);
	pthread_cond_init(&notFull, NULL);
	pthread_cond_init(&notEmpty, NULL);
}


void produce() {
 	pthread_once(&is_initialized, init);
	pthread_mutex_lock(&bufLock);
	while(count == MAX_SIZE)
		pthread_cond_wait(&notFull, &bufLock); 
	buf[count] = getChar();
	count++;
	pthread_cond_signal(&notEmpty);
	pthread_mutex_unlock(&bufLock);
}	

void consume() {
 	pthread_once(&is_initialized, init);
	pthread_mutex_lock(&bufLock);
	while(count == 0)
		pthread_cond_wait(&notEmpty, &bufLock);
	useChar(buf[count-1]);
	count--;
	pthread_cond_signal(&notFull);
	pthread_mutex_unlock(&bufLock);
}

