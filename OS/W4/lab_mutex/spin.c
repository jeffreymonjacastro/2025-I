#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

int lock_val = 0;

void lock() {
  int c;
  while ((c = __sync_val_compare_and_swap(&lock_val, 0, 1)) != 0);
}

void unlock() {
  lock_val = 0;
}

void* f_thread0(void *v) {
  lock();
  printf("Thread 0.\n");
  sleep(5);
  unlock();
  return NULL;
}

void* f_thread1(void *v) {
  lock();
  printf("Thread 1.\n");
  sleep(5);
  unlock();
  return NULL;
}

int main() {
  pthread_t thr0, thr1; 

  pthread_create(&thr0, NULL, f_thread0, NULL);
  pthread_create(&thr1, NULL, f_thread1, NULL);   
  
  pthread_exit(0);
}
