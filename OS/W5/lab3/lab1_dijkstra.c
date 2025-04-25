#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BIG 100000
#define N_THR 11

int count = 0;
int order = -1;
int minus_one = -1;

int interest[N_THR];

int check_others(int me) {
  for (int i = 0; i < N_THR; i++) {
    if (i != me && interest[i] > 0) {
      return 1;
    }
  }
  return 0;
}

void* thr(void* arg) {
  int me = *((int*) arg);
  
  printf("Thread %d\n", me);
  
  for (int i = 0; i < BIG; i++){
    __atomic_fetch_add(&interest[me], 1, __ATOMIC_SEQ_CST);
    while (check_others(me)) {
      if (order != me) {
        __atomic_fetch_sub(&interest[me], 1, __ATOMIC_SEQ_CST);
        while (order != -1); 
        __atomic_store(&order, &me, __ATOMIC_SEQ_CST);
        __atomic_fetch_add(&interest[me], 1, __ATOMIC_SEQ_CST);
      }
    }
    
    count++;
    __atomic_store(&order, &minus_one, __ATOMIC_SEQ_CST);
    __atomic_fetch_sub(&interest[me], 1, __ATOMIC_SEQ_CST);
  }
}

int main(void){
  pthread_t threads[N_THR];
  int ids[N_THR];

  for (int i = 0; i < N_THR; i++) {
    interest[i] = 0;
    ids[i] = i;
    pthread_create(&threads[i], NULL, thr, &ids[i]);
  }

  for (int i = 0; i < N_THR; i++) {
    pthread_join(threads[i], NULL);
  }
  
  printf("Counter: %d\n", count);
}