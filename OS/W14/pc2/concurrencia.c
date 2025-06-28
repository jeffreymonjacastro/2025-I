#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define N 5    // Producers
#define M 10   // Consumers
#define TOT 3

int* buffer; 
int id_prod[N];
int id_cons[M];

int buffer_id = 0;  // Contador que indicará la posición del buffer

pthread_mutex_t lock_prod, lock_cons, lock;
pthread_cond_t cond_prod, cond_cons;

void* producer(void* args){
  int id = *(int*)args;

  while(1){
    pthread_mutex_lock(&lock_prod); // Solo un productor puede producir a la vez
    
    while (buffer_id == TOT) { // Si el bufer está lleno, sleep
      pthread_cond_wait(&cond_prod, &lock_prod);
    }

    printf("Producer %d adds a Inka cola on buffer %d\n", id, buffer_id);
    sleep(1);

    pthread_mutex_lock(&lock);    
    buffer[buffer_id] = 1; // Añade una Inka cola en la posición buffer_id
    buffer_id++; // Aumenta el contador
    pthread_mutex_unlock(&lock);    

    if (buffer_id == TOT){ // Si el buffer se llena, despierta a todos los consumers
      pthread_cond_broadcast(&cond_cons);
    }

    pthread_mutex_unlock(&lock_prod);
  }

  return NULL;
}

void* consumer(void* args){
  int id = *(int*)args;

  while(1){
    pthread_mutex_lock(&lock_cons); // Solo un consumidor puede consumir a la vez
    
    while (buffer_id == 0) { // Si el buffer está vacío, sleep
      pthread_cond_wait(&cond_cons, &lock_cons);
    }

    printf("Consumer %d drinks a Inka cola on buffer %d\n", id, buffer_id);
    sleep(1);

    pthread_mutex_lock(&lock);    
    buffer[buffer_id] = 0; // Bebe una Inka cola en la posición buffer_id
    buffer_id--; // Disminuye el contador
    pthread_mutex_unlock(&lock);    

    if (buffer_id == 0) { // Si el buffer se vacía, despierta a todos los producers
      pthread_cond_broadcast(&cond_prod);
    }

    pthread_mutex_unlock(&lock_cons);
  }

  return NULL;
}


int main(){
  pthread_t producers[N], consumers[M]; // N threads de producers, M threads de consumer
  buffer = malloc(TOT * sizeof(int)); // Buffer en heap
  
  // Inicialización
  pthread_mutex_init(&lock_prod, NULL);
  pthread_mutex_init(&lock_cons, NULL);
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond_prod, NULL);
  pthread_cond_init(&cond_cons, NULL);

  for (int i = 0; i < N; i++){
    id_prod[i] = i;
    pthread_create(&producers[i], NULL, producer, (void*)&id_prod[i]);
  }

  for (int i = 0; i < M; i++){
    id_cons[i] = i;
    pthread_create(&consumers[i], NULL, consumer, (void*)&id_cons[i]);
  }

  for (int i = 0; i < N; i++){
    pthread_join(producers[i], NULL);
  }

  for (int i = 0; i < M; i++){
    pthread_join(consumers[i], NULL);
  }

  // free
  pthread_mutex_destroy(&lock_prod);
  pthread_mutex_destroy(&lock_cons);
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond_prod);
  pthread_cond_destroy(&cond_cons);

  return 0;
}