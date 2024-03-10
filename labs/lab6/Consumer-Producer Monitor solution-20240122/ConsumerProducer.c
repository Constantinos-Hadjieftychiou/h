#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include "ConsumerProducerMonitor.h"

void producer()
{
	int i = 0;
	for (;;)
	{
		produce();
		// Added a random wait to show thread behaviour
		if (++i % 10 == 0)
			sleep((rand() % 3) + 5);
		else
			usleep(((rand() % 10) + 1) * 100000);
	}
}

void consumer()
{
	int i = 0;
	for (;;)
	{
		consume();
		// Added a random wait to show thread behaviour
		if (++i % 10 == 0)
			sleep((rand() % 3) + 1);
		else
			usleep(((rand() % 16) + 5) * 100000);
	}
}

int main()
{
	pthread_t p, c;

	pthread_create(&p, NULL, (void *)producer, NULL);
	pthread_create(&c, NULL, (void *)consumer, NULL);

	pthread_join(p, NULL);
	pthread_join(c, NULL);

	return 0;
}
