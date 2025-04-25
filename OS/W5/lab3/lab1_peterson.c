#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BIG 100000

int count = 0;
int order = 0;

int interest[2] = {0, 0};

void* thr0(void* arg) {
  printf("Thread 0\n");

  int me = 0;  
  int other = 1;
  
  for (int i = 0; i < BIG; i++){
    __atomic_fetch_add(&interest[me], 1, __ATOMIC_SEQ_CST);
    __atomic_store(&order, &other, __ATOMIC_SEQ_CST);
    while (interest[other] && order != me);
    count++;
    __atomic_fetch_sub(&interest[me], 1, __ATOMIC_SEQ_CST);
  }
}

void* thr1(void* arg) {
  printf("Thread 1\n");

  int me = 1;  
  int other = 0;
  
  for (int i = 0; i < BIG; i++){
    __atomic_fetch_add(&interest[me], 1, __ATOMIC_SEQ_CST);
    __atomic_store(&order, &other, __ATOMIC_SEQ_CST);
    while (interest[other] && order != me);
    count++;
    __atomic_fetch_sub(&interest[me], 1, __ATOMIC_SEQ_CST);
  }
}

// void* thr(void* arg){
//   int me = *((int*) arg);
//   int other = 1 - me;
// }

int main(void){
  pthread_t peterson1, peterson2;
  
  pthread_create(&peterson1, NULL, thr0, NULL);
  pthread_create(&peterson2, NULL, thr1, NULL);

  pthread_join(peterson1, NULL);
  pthread_join(peterson2, NULL);

  printf("Counter: %d\n", count);
}