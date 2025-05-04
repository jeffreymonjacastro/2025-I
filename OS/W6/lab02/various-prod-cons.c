#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define N 10
#define PRODUCERS 2
#define CONSUMERS 3

int buffer[N];
int i = 0, f = 0;
int next = 0;
sem_t full, empty;
pthread_mutex_t lock_prod, lock_con;

int produce(int id) {
	printf("Producer %d, item = %d\n", id, ++next);
	return next;
};

int consume(int id, int *item) {
	int content = *item;
	*item = -1;
	printf("Consumer %d, item = %d\n", id, content);
	return content;
};

void *prod (void *arg) {
	int id = *((int *) arg);

	while (1) {
		sleep(2);
		sem_wait(&empty);
		pthread_mutex_lock(&lock_prod);
		f = (f + 1) % N;
		buffer[f] = produce(id);
		pthread_mutex_unlock(&lock_prod);
		sem_post(&full);
	}
}

void *cons (void *arg) {
	int id = *((int *) arg);
	printf("Consumer %d\n", id);

	while (1) {
		sleep(2);
		sem_wait(&full);
		pthread_mutex_lock(&lock_con);
		i = (i + 1) %N;
		consume(id, &buffer[i]);
		pthread_mutex_unlock(&lock_con);
		sem_post(&empty);
	}
}

int main () {
	pthread_t p[PRODUCERS], c[CONSUMERS];

	sem_init (&full, 0, 0);
	sem_init (&empty, 0, N);
	pthread_mutex_init(&lock_prod, NULL);
	pthread_mutex_init(&lock_con, NULL);

	memset (buffer, -1, sizeof (int) * N);

	for (int i = 0; i < PRODUCERS; i++) {
		int *id = &i;
		pthread_create(&p[i], NULL, prod, id);
	}

	for (int i = 0; i < CONSUMERS; i++) {
		int *id = &i;
		pthread_create(&c[i], NULL, cons, id);
	}

	for (int i = 0; i < PRODUCERS; i++) {
		pthread_join(p[i], NULL);
	}

	for (int i = 0; i < CONSUMERS; i++) {
		pthread_join(c[i], NULL);
	}

	return 0;
}
