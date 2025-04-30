#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_t thread_id[2];
int counter=0;
pthread_mutex_t lock;

void* fthread(void* arg)
{
	pthread_mutex_lock(&lock);

	counter += 1;
	printf("\n Thread %d in critical section\n", counter);

	unsigned long i = 0;
	for (i = 0; i < (0xFFFFFFFF); i++)
		;

	printf("\n Thread %d leaving critical section\n", counter);

	pthread_mutex_unlock(&lock);

	return NULL;
}

int main(void)
{
	pthread_mutex_init(&lock, NULL);

	pthread_create(&(thread_id[0]),NULL,&fthread, NULL);
	pthread_create(&(thread_id[1]),NULL,&fthread, NULL);

	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
	pthread_mutex_destroy(&lock);

	return 0;
}

