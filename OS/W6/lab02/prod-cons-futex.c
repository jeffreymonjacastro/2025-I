#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <linux/futex.h>
#include <sys/syscall.h>

#define N 10

int buffer[N];
int i = 0, f = 0;
int c = 0;
int next = 0;

long futex_wait (int *uaddr, int val) {
	return syscall (SYS_futex, uaddr, FUTEX_WAIT, val, NULL, NULL, 0);
}

long futex_wake (int *uaddr, int val) {
	return syscall (SYS_futex, uaddr, FUTEX_WAKE, val, NULL, NULL, 0);
};

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
		sleep (2);
		while (c == N) {
			futex_wait (&c, N);
		}
		f = (f + 1) % N;
		buffer[f] = produce ();
		c++; // make it atomic
		if (c == 1) {
			futex_wake (&c, 1);
		}
	}
}

void *cons (void *arg) {
	while (1) {
		sleep (1);
		while (c == 0) {
			futex_wait (&c, 0);
		}
		i = (i + 1) %N;
		consume (&buffer[i]);
		c--;
		if (c == N - 1) {
			futex_wake (&c, 1);
		}
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
