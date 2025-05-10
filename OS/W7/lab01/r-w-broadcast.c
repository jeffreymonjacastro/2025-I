/*
 * Readers writers with locks and cond variables
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile int bd = -1; /* data base */

/* Suggested variables */
volatile int nr = 0;
volatile int nw = 0;

/* for exec no error check */
pthread_mutex_t lock_cont = PTHREAD_MUTEX_INITIALIZER; 

pthread_cond_t cond_wr = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_rd = PTHREAD_COND_INITIALIZER;

void *reader(void* v) {
  int id = *((int *) v);

  while (1){
  pthread_mutex_lock(&lock_cont); /* Mutex on access to monitor's resources */
  while (nw > 0) pthread_cond_wait(&cond_rd, &lock_cont); /* We hold the lock, we can release it */
  nr++;
  pthread_mutex_unlock(&lock_cont);
  
  sleep(1);
  printf("reader %d read value %d \n", id, bd);
  
  pthread_mutex_lock(&lock_cont);
  nr--;
  if (nr == 0) 
    pthread_cond_signal(&cond_wr);
  pthread_mutex_unlock(&lock_cont);
  }
  return NULL;
}

void *writer(void *v) {
  int id = *((int *) v);

  while(1){
  pthread_mutex_lock(&lock_cont); /* Mutex on access to monitor's resources */
  while (nr > 0 || nw > 0) pthread_cond_wait(&cond_wr, &lock_cont);
  nw++;
  pthread_mutex_unlock(&lock_cont);
  
  bd = id;
  sleep(1);
  printf("writer %d wrote value %d \n", id, bd);
  
  pthread_mutex_lock(&lock_cont);
  nw--;
  pthread_cond_broadcast(&cond_rd);  
  pthread_cond_signal(&cond_wr);
  pthread_mutex_unlock(&lock_cont);
  }
  return NULL;
}

#define N 100
int main() {
  pthread_t thr[N];
  int tid[N];
  int i = 0, r = 0, w = 0;

  srandom(time(NULL));
  for (i = 0; i < N; i++) {
    if (random() % 2) {
	  tid[i] = r;
	  r++;
      pthread_create(&thr[i], NULL, reader, (void*) &tid[i]);
    }
    else {
      tid[i] = w;
	  w++;
      pthread_create(&thr[i], NULL, writer, (void*) &tid[i]);
    }
  }

  pthread_exit(NULL);
}
