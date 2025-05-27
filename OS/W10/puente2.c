#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 10

pthread_mutex_t lock;
pthread_cond_t puente;
int carros[2] = {0, 0}; // norte-sur, sur-norte
int waiting[N] = {N+1};
int ticket = 0;

int am_i_min(int id){
  for (int i = 0; i < N; i++){
    if (waiting[i] < waiting[id]) 
      return 0;
  }
  return 1;
}

void cruzar(int id, int sentido) { 
  pthread_mutex_lock(&lock);
  
  while (carros[sentido] > 0 || am_i_min(id) == 0) {
    printf("Esperando %d...", id);
    pthread_cond_wait(&puente, &lock);
  }
  
  carros[sentido]++;
  printf("Carro %d con dirección %s cruzando el puente\n", id, sentido ? "sur-norte" : "norte-sur");
  printf("A: %d, B: %d \n", carros[0], carros[1]);
  
  pthread_mutex_unlock(&lock);
}

void salir(int id, int sentido) {
  pthread_mutex_lock(&lock);

  carros[sentido]--;
  printf("Carro %d ya cruzó\n", id);
  waiting[id] = N + 1;

  pthread_cond_broadcast(&puente);

  pthread_mutex_unlock(&lock);    
}

void* carro(void* args) {
  int id = *(int*) args;
  int sentido = rand() % 2;

  waiting[id] = ticket++;

  cruzar(id, sentido);
  sleep(1);
  salir(id, sentido);
}

int main() {
  pthread_t carros[N];
  srand(time(NULL)); 

  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&puente, NULL);
  
  int ids[N];
  for (int i = 0; i < N; i++) {
    ids[i] = i;
    pthread_create(&carros[i], NULL, &carro, (void*)&ids[i]);
  }

  for (int i = 0; i < N; i++) {
    pthread_join(carros[i], NULL);
  }

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&puente);
  
  return 0;
}
