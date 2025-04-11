#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NTHREADS 3

pthread_barrier_t barrier;

void *routine(void *args) {
	while (1) {
		printf("Waiting at the barrier...\n");
		sleep(1);
		pthread_barrier_wait(&barrier);
		printf("We passed the barrier\n");
		sleep(1);
	}
}

int main (int argc, char *argv[]) {
	pthread_t th[NTHREADS];
	int i;
	pthread_barrier_init(&barrier, NULL, NTHREADS);

	for (i = 0; i < NTHREADS; i++) {
		pthread_create(&th[i], NULL, &routine, NULL);
	}

	for (i = 0; i < NTHREADS; i++) {
		pthread_join(th[i], NULL);
	}

	pthread_barrier_destroy(&barrier);
	return 0;
}
