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
    while (interest[other]) {
      if (order != me) {
        __atomic_fetch_sub(&interest[me], 1, __ATOMIC_SEQ_CST);
        while (order != me); 
        __atomic_fetch_add(&interest[me], 1, __ATOMIC_SEQ_CST);
      }
    }
    count++;

    order = other;
    __atomic_fetch_sub(&interest[me], 1, __ATOMIC_SEQ_CST);
  }
}

void* thr1(void* arg) {
  printf("Thread 1\n");

  int me = 1;  
  int other = 0;
  
  for (int i = 0; i < BIG; i++){
    __atomic_fetch_add(&interest[me], 1, __ATOMIC_SEQ_CST);
    while (interest[other]) {
      if (order != me) {
        __atomic_fetch_sub(&interest[me], 1, __ATOMIC_SEQ_CST);
        while (order != me); 
        __atomic_fetch_add(&interest[me], 1, __ATOMIC_SEQ_CST);
      }
    }
    count++;

    order = other;
    __atomic_fetch_sub(&interest[me], 1, __ATOMIC_SEQ_CST);
  }
}

// void* thr(void* arg){
//   int me = *((int*) arg);
//   int other = 1 - me;
// }

int main(void){
  pthread_t dekker1, dekker2;
  
  pthread_create(&dekker1, NULL, thr0, NULL);
  pthread_create(&dekker2, NULL, thr1, NULL);

  pthread_join(dekker1, NULL);
  pthread_join(dekker2, NULL);

  printf("Counter: %d\n", count);
}