#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define N 10

int buffer[N];
int i = 0, f = 0;
int next = 0;
sem_t full, empty;

int produce () {
	printf ("Producer, item = %d\n", ++next);
	return next;
};

int consume (int *item) {
	int content = *item;
	*item = -1;
	printf ("Consumer, item = %d\n", content);
	return content;
};

void *prod (void *arg) {
	while (1) {
		sleep (1);
		sem_wait (&empty);
		f = (f + 1) % N;
		buffer[f] = produce ();
		sem_post (&full);
	}
}

void *cons (void *arg) {
	while (1) {
		sleep (1);
		sem_wait (&full);
		i = (i + 1) %N;
		consume (&buffer[i]);
		sem_post (&empty);
	}
}

int main () {
	pthread_t p, c;

	sem_init (&full, 0, 0);
	sem_init (&empty, 0, N);
	memset (buffer, -1, sizeof (int) * N);

	pthread_create (&p, NULL, prod, NULL);
	pthread_create (&c, NULL, cons, NULL);

	pthread_join (p, NULL);
	pthread_join (c, NULL);

	return 0;
}
