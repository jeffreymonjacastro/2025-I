/*
 * R/W with semaphores.
 * Multiple readers is possible, but
 * writers can starve.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem_bd;
volatile int bd = -1; /* data base */

sem_t sem_nr;
volatile int nr = 0;

void *reader(void* v) {
  int id = *((int *) v);

  sem_wait(&sem_nr);
  nr++;
  if (nr == 1) /* If we are first reader, we block writers */
    sem_wait(&sem_bd);
  sem_post(&sem_nr);
  
  printf("reader %d read value %d \n", id, bd);
  sleep(1);
  
  sem_wait(&sem_nr);
  nr--;
  if (nr == 0)
    sem_post(&sem_bd);
  sem_post(&sem_nr);
  
  return NULL;
}

void *writer(void *v) {
  int id = *((int *) v);
  
  sem_wait(&sem_bd); /* While there are readers, we wait */
  bd = id;
  sleep(1);
  printf("writer %d wrote value %d \n", id, bd);
  sem_post(&sem_bd);

  return NULL;
}

#define N 300
int main() {
  pthread_t thr[N];
  int i = 0, r = 0, w = 0;

  sem_init(&sem_bd, 0, 1);
  sem_init(&sem_nr, 0, 1);
  
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

