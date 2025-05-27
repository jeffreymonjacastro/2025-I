#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define N 5 
#define H 5
#define MAX_ITER 20

int tarro = 0;

sem_t mutex;
sem_t tarro_lleno;
sem_t oso_comiendo;
sem_t llenando_tarro;

void *abeja(void* args){
  int id = *(int*)args;

  for (int i = 0; i < MAX_ITER / N; i++) {
    sem_wait(&oso_comiendo);
    sem_wait(&mutex);
    
    sem_wait(&llenando_tarro);
    printf("Abejita %d llenando el tarro...\n", id);
    tarro++;
    sem_post(&llenando_tarro);

    if (tarro == H) {
      sem_post(&tarro_lleno);
      sem_wait(&oso_comiendo);
    } 

    sem_post(&oso_comiendo);
  }
}

void *oso(){
  for (int i = 0; i < MAX_ITER / H; i++) {
    sem_wait(&tarro_lleno);

    printf("El oso esta comiendo...\n");
    sleep(1);

    tarro = 0; 

    for (int i = 0; i < H; i++) {
      sem_post(&mutex); 
    }

    sem_post(&oso_comiendo);
  }
}

int main (){
  sem_init(&mutex, 0, H);
  sem_init(&tarro_lleno, 0, 0);
  sem_init(&oso_comiendo, 0, 1);
  sem_init(&llenando_tarro, 0, 1);

  pthread_t abejas[N];
  pthread_t osito;

  pthread_create(&osito, NULL, oso, NULL);

  int ids[N];
  for (int i = 0; i < N; i++) {
    ids[i] = i;
    pthread_create(&abejas[i], NULL, abeja, (void*) &ids[i]);
  }

  pthread_join(osito, NULL);

  for (int i = 0; i < N; i++) {
    pthread_join(abejas[i], NULL);
  }

  sem_destroy(&mutex);
  sem_destroy(&tarro_lleno);
  sem_destroy(&oso_comiendo);
  return 0;
}