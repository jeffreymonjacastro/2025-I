#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_t thread_id[2];
sem_t mutex;

void* thread(void* arg)
{
	//wait
	sem_wait(&mutex);
	printf("Thread %d in critical section\n", *((int *) arg));

	//critical section
	sleep(4);

	//signal
	printf("Thread %d leaving critical section\n", *((int *) arg));
	sem_post(&mutex);

	return NULL;
}


int main()
{
	int tid0 = 0, tid1 = 1;

	sem_init(&mutex, 0, 1);

	pthread_create(&thread_id[0],NULL,thread, (void *) &tid0);
	sleep(2);
	pthread_create(&thread_id[1],NULL,thread, (void *) &tid1);
	
	pthread_join(thread_id[0],NULL);
	pthread_join(thread_id[1],NULL);
	
	sem_destroy(&mutex);
	return 0;
}

