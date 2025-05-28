#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5  
#define N_ITER 1

enum {THINKING, HUNGRY, EATING} estado[N];

pthread_mutex_t mutex;

sem_t filo[N];

void test(int i) {
	if ((estado[(i + 4) % N] != EATING) && 
			(estado[i] == HUNGRY) && 
			(estado[(i + 1) % N] != EATING)) {
			
		estado[i] = EATING;
		printf("Filósofo %d está COMIENDO\n", i);
		sem_post(&filo[i]); 
	}
}

void pickup(int i) {
	pthread_mutex_lock(&mutex);
	
	estado[i] = HUNGRY;
	printf("Filósofo %d tiene HAMBRE\n", i);
	
	test(i);
	
	pthread_mutex_unlock(&mutex);
	
	sem_wait(&filo[i]);
}

void putdown(int i) {
	pthread_mutex_lock(&mutex);
	
	estado[i] = THINKING;
	printf("Filósofo %d está PENSANDO\n", i);
	
	test((i + 4) % N);  
	test((i + 1) % N);  
	
	pthread_mutex_unlock(&mutex);
}

void* philosopher(void* arg) {
	int id = *(int*)arg;

	for (int i = 0; i < N_ITER; i++){
		sleep(1);
		pickup(id);
		sleep(1);
		putdown(id);
	}
	return NULL;
}

int main() {
	pthread_t filosofos[N];
	
	pthread_mutex_init(&mutex, NULL);
	
	for (int i = 0; i < N; i++) {
		estado[i] = THINKING;
		sem_init(&filo[i], 0, 0);  
	}
	
	int ids[N];
	for (int i = 0; i < N; i++) {
		ids[i] = i;
		pthread_create(&filosofos[i], NULL, philosopher, &ids[i]);
	}
	
	for (int i = 0; i < N; i++) {
		pthread_join(filosofos[i], NULL);
	}
	
	pthread_mutex_destroy(&mutex);

	for (int i = 0; i < N; i++) {
		sem_destroy(&filo[i]);
	}
	
	return 0;
}