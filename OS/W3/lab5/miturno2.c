#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *miTurno(void *arg) {
	for (size_t i = 0; i < 8; i++)
	{
		sleep(1);
		printf("Mi turno! %lu\n", i);
	}

	return NULL;
}

void tuTurno() {
	for (size_t i = 0; i < 3; i++) {
		sleep(2);
		printf("Tu turno! %lu\n", i);
	}
}

int main () {
	pthread_t miThread;

	pthread_create(&miThread, NULL, miTurno, NULL);
	tuTurno();

	return 0;
}
