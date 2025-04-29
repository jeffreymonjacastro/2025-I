/*
 * Example 1: FUTEX_WAKE. Thread 0 wakes up thread 1.
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

#define ACQUIRED 0
#define FREE 1

int futex_addr = ACQUIRED;

/* Returns -1 if futex has not blocked and
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

void* f_thread1(void *v) {
  futex_addr = ACQUIRED;
  /* Will wait while futex value is the same as val1 */
  futex_wait(&futex_addr, ACQUIRED);
  printf("Thread 1.\n");
  return NULL;
}

void* f_thread0(void *v) {
  sleep(3);
  /* Wake up 1 thread */
  futex_wake(&futex_addr, 1);  
  printf("Thread 0.\n");
  return NULL;
}

int main() {
  pthread_t thr0, thr1; 

  pthread_create(&thr0, NULL, f_thread0, NULL);
  pthread_create(&thr1, NULL, f_thread1, NULL);
  
  pthread_exit(0);
}
