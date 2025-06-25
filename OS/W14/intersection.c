#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 10

pthread_mutex_t lock, sem_lock;
pthread_cond_t semaphore[4];

int wait[4] = {0}; // 0: N, 1: S, 2: E, 3: O (cantidad de carros esperando por direccion)

int curr_dir = 0;

void print_status() {
  printf("Estado actual: ");
  for (int i = 0; i < 4; i++) {
    printf("(%d: %d) -> ", i, wait[i]);
  }
  printf("\n");
}

void entrada(int dir, int id) {
  pthread_mutex_lock(&lock);
  wait[dir]++;
  pthread_mutex_unlock(&lock);
}

void salida(int dir, int id) {
  pthread_mutex_lock(&lock);

  while (curr_dir != dir) {
    pthread_cond_wait(&semaphore[dir], &lock);
  }

  // print_status();
  sleep(1);
  wait[dir]--;
  pthread_mutex_unlock(&lock);
}

void *carro(void *args) {
  for (int i = 0; i < 1; i++) {
    int dir = rand() % 4;
    int me = *(int *)args;

    printf("Carro %d quiere cruzar desde la direccion %d\n", me, dir);
    entrada(dir, me);
    salida(dir, me);
    printf("Carro %d termino de cruzar\n", me);
  }
  return NULL;
}

void *semaforo() {
  while (1) {
    pthread_mutex_lock(&sem_lock);

    for (int i = 0; i < 5; i++) {
      if (wait[i] == 0) {
        break;
      }

      printf("Semáforo en dirección %d abierto, permitiendo pasar %d carros\n", curr_dir, wait[curr_dir]);
      pthread_cond_broadcast(&semaphore[curr_dir]);
      sleep(1);
    }

    curr_dir = (curr_dir + 1) % 4;

    pthread_mutex_unlock(&sem_lock);
  }
}

int main() {
  pthread_t carros[N], semaforo_thread;
  int idx[N];

  srand(time(NULL));

  pthread_mutex_init(&lock, NULL);
  pthread_mutex_init(&sem_lock, NULL);

  for (int i = 0; i < 4; i++) {
    pthread_cond_init(&semaphore[i], NULL);
  }

  for (int i = 0; i < N; i++) {
    idx[i] = i;
    pthread_create(&carros[i], NULL, &carro, (void *)&idx[i]);
  }

  pthread_create(&semaforo_thread, NULL, &semaforo, NULL);

  for (int i = 0; i < N; i++) {
    pthread_join(carros[i], NULL);
  }

  pthread_cancel(semaforo_thread);
  pthread_mutex_destroy(&lock);

  for (int i = 0; i < 4; i++) {
    pthread_cond_destroy(&semaphore[i]);
  }

  pthread_exit(0);

  return 0;
}