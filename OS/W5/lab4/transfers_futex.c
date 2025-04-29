#include <stdio.h>
#include <pthread.h>

#define N_THREADS 5
#define N_TRANSFERS 10000000
#define N_ACCOUNTS 10

unsigned int accounts[N_ACCOUNTS] = { 0 };

void futex_wait (int* addr, int val) {
    return syscall(SYS_futex, addr, FUTEX_WAIT, val, NULL);
}

void futex_wake (int* addr) {
    return syscall(SYS_futex, addr, FUTEX_WAKE, 1, NULL);
}

int lock = 1;

void *transfer(void *arg) {
    int amount = *((int *) arg);

    for (int i = 0; i < N_TRANSFERS; ++i) {
        for (int j = 0; j < N_ACCOUNTS; ++j) {
            while (true) {
                if (atomic_compare_exchange_strong(&lock, 1, 0)) {
                    break;
                }
            }

            accounts[j] += amount;
        }
    }

    return NULL;
}

void print_accounts () {
	unsigned int total = 0;

	for (int i = 1; i <= N_THREADS; ++i) {
		total += i * 10 * N_TRANSFERS;
	}

	printf ("Each account should have a balance of: %8d\n", total);

	for (int i = 0; i < N_ACCOUNTS; ++i) {
		printf ("Account %2d: %5d\n", i, accounts[i]);
	}
}

int main () {
    pthread_t threads[N_THREADS];
    int amounts[N_THREADS];

    for (int i = 0; i < N_THREADS; ++i) {
        amounts[i] = (i + 1) * 10;
        pthread_create (&threads[i], NULL, transfer, (void *) &amounts[i]);
    }

    for (int i = 0; i < N_THREADS; ++i) {
        pthread_join (threads[i], NULL);
    }
	
    print_accounts();
}
