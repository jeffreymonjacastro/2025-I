/*
 * Readers writers with locks and cond variables.
 * No starvation.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile int bd = -1; /* Data base */

volatile int nr = 0;

sem_t lock_bd;
pthread_mutex_t lock_nr = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *reader(void* v) {
  int id = *((int *) v);

  sem_wait(&lock_bd); /* if writer is interested, we wait */
  pthread_mutex_lock(&lock_nr); /* mutex for readers count */
  nr++;
  pthread_mutex_unlock(&lock_nr);
  sem_post(&lock_bd);
  
  sleep(1);
  printf("reader %d read value %d \n", id, bd);
  
  pthread_mutex_lock(&lock_nr);
  nr--;
  if (nr == 0) /* no more readers, we signal the writers */
    pthread_cond_signal(&cond);
  pthread_mutex_unlock(&lock_nr);
  
  return NULL;
}

void *writer(void *v) {
  int id = *((int *) v);
  
  sem_wait(&lock_bd); /* Grabbing this lock gives writers priority */
  pthread_mutex_lock(&lock_nr);

  while (nr > 0) /* We must wait until no readers are left */
    pthread_cond_wait(&cond, &lock_nr); /* lock_nr is released, but we keep lock_bd */
  
  bd = id;
  sleep(2);
  printf("writer %d wrote value %d \n", id, bd);
  
  pthread_mutex_unlock(&lock_nr);
  sem_post(&lock_bd);

  return NULL;
}

#define N 60
int main() {
  sem_init (&lock_bd, 0, 1);

  pthread_t thr[N];
  int i = 0, r = 0, w = 0;

  for (i = 0; i < N; i++) {
    if (random() % 2) {
      r++;
      pthread_create(&thr[i], NULL, reader, (void*) &r);
    }
    else {
      w++;
      pthread_create(&thr[i], NULL, writer, (void*) &w);
    }
  }

  pthread_exit(NULL);
}
