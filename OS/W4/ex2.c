/*
 * Example 2: FUTEX_WAKE. Thread 0 wakes up N-1 threads.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <limits.h>

#define N 10
#define ACQUIRED 0
#define FREE 1

int futex_addr = ACQUIRED;

/* Returns -1 if futex have not blocked and
            0 otherwise */
int futex_wait(void *addr, int val1) {
  return syscall(SYS_futex, addr, FUTEX_WAIT, 
                 val1, NULL, NULL, 0);
}

/* Returns the number of running threads (just woke up) */
int futex_wake(void *addr, int n) {
    return syscall(SYS_futex, addr, FUTEX_WAKE, 
                   n, NULL, NULL, 0);
}

void* f_thread(void *v) {
  futex_wait(&futex_addr, ACQUIRED);
  printf("Thread %d.\n", *((int *) v));
  return NULL;
}

void* f_thread0(void *v) {
  sleep(5);
  futex_wake(&futex_addr, INT_MAX); /*  N-1 */  
  printf("Thread 0.\n");
  return NULL;
}

int main() {
  pthread_t thr[N];
  int tid[N];
  int i;
  
  for (i=1; i < N; i++) {
    tid[i] = i;
    pthread_create(&thr[i], NULL, f_thread, (void*) &tid[i]);
  }
  pthread_create(&thr[0], NULL, f_thread0, NULL);
  
  pthread_exit(0);
}
