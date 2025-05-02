#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define N 10

int buffer[N];
int i = 0, f = 0;
int c = 0;
int next = 0;

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
		while (c == N); // Don't produce if buffer is full
		f = (f + 1) % N;
		buffer[f] = produce ();
		asm("movl c(%rip), %ebx\n"
			"addl $1, %ebx\n");
		sleep (2);
		asm("movl %ebx, c(%rip)\n");

	}
}

void *cons (void *arg) {
	int item;
	sleep (5);
	while (1) {
		sleep (1);
		while (c == 0); // Don't consume if buffer is empty
		i = (i + 1) %N;
		item = consume (&buffer[i]);
		c--;
	}
}

int main () {
	pthread_t p, c;

	memset (buffer, -1, sizeof (int) * N);

	pthread_create (&p, NULL, prod, NULL);
	pthread_create (&c, NULL, cons, NULL);

	pthread_join (p, NULL);
	pthread_join (c, NULL);

	return 0;
}
