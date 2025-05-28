#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N_iter 5

pthread_cond_t listo;
pthread_cond_t pastelero[3];
pthread_mutex_t lock;

int ingredientes[3] = {0, 0, 0}; // harina, huevo, azucar
int listo_cont = 0;
int finished = 0;

void* chef(){
  for (int i = 0; i < N_iter; i++) {
    pthread_mutex_lock(&lock);

    while (listo_cont == 1) {
      pthread_cond_wait(&listo, &lock);
    }

    listo_cont = 1;
    int r = rand() % 3;

    if (r == 0) {
      printf("Chef: Añadiendo harina y huevo\n");
      ingredientes[0]++; // harina
      ingredientes[1]++; // huevo
    } else if (r == 1) {
      printf("Chef: Añadiendo huevo y azucar\n");
      ingredientes[1]++; // huevo
      ingredientes[2]++; // azucar
    } else if (r == 2) {
      printf("Chef: Añadiendo azucar y harina\n");
      ingredientes[2]++; // azucar
      ingredientes[0]++; // harina
    }
  
    pthread_cond_signal(&pastelero[r]);
    pthread_mutex_unlock(&lock);
  }

  pthread_mutex_lock(&lock);
  finished = 1;
  printf("Chef: Trabajo terminado, notificando a todos los pasteleros\n");
  pthread_cond_broadcast(&pastelero[0]);
  pthread_cond_broadcast(&pastelero[1]);
  pthread_cond_broadcast(&pastelero[2]);
  pthread_mutex_unlock(&lock);
}

void* pastelero_harina_huevo(){
  while(1) {
    pthread_mutex_lock(&lock);

    while (!ingredientes[0] || !ingredientes[1]) {
      pthread_cond_wait(&pastelero[0], &lock);
      if (finished) {
        printf("Pastelero harina-huevo: Chef terminó mientras esperaba, saliendo\n");
        pthread_mutex_unlock(&lock);
        return NULL;
      }
    }
    
    printf("Pastelero: Preparando pastel con harina y huevo\n");
    ingredientes[0]--;
    ingredientes[1]--;
    sleep(1);
    listo_cont = 0;
    pthread_cond_signal(&listo);
    pthread_mutex_unlock(&lock);
  }
}

void* pastelero_huevo_azucar(){
  while(1) {
    pthread_mutex_lock(&lock);

    while (!ingredientes[1] || !ingredientes[2]) {
      pthread_cond_wait(&pastelero[1], &lock);
      if (finished) {
        printf("Pastelero huevo-azucar: Chef terminó mientras esperaba, saliendo\n");
        pthread_mutex_unlock(&lock);
        return NULL;
      }
    }

    printf("Pastelero: Preparando pastel con huevo y azucar\n");
    ingredientes[1]--;
    ingredientes[2]--;
    sleep(1);
    listo_cont = 0;
    pthread_cond_signal(&listo);
    pthread_mutex_unlock(&lock);
  }
}

void* pastelero_azucar_harina(){
  while(1) {
    pthread_mutex_lock(&lock);

    while (!ingredientes[2] || !ingredientes[0]) {
      pthread_cond_wait(&pastelero[2], &lock);
      if (finished) {
        printf("Pastelero azucar-harina: Chef terminó mientras esperaba, saliendo\n");
        pthread_mutex_unlock(&lock);
        return NULL;
      }
    }

    printf("Pastelero: Preparando pastel con azucar y harina\n");
    ingredientes[2]--;
    ingredientes[0]--;
    sleep(1);
    listo_cont = 0;
    pthread_cond_signal(&listo);
    pthread_mutex_unlock(&lock);
  }
}

int main(){
  srand(time(NULL));
  
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&listo, NULL);

  for (int i = 0; i < 3; i++) {
    pthread_cond_init(&pastelero[i], NULL);
  } 

  pthread_t chefsito;
  pthread_t pastelero1, pastelero2, pastelero3;

  pthread_create(&chefsito, NULL, chef, NULL);
  pthread_create(&pastelero1, NULL, pastelero_harina_huevo, NULL);
  pthread_create(&pastelero2, NULL, pastelero_huevo_azucar, NULL);
  pthread_create(&pastelero3, NULL, pastelero_azucar_harina, NULL);

  pthread_join(chefsito, NULL);
  pthread_join(pastelero1, NULL);
  pthread_join(pastelero2, NULL);
  pthread_join(pastelero3, NULL);

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&listo);

  for (int i = 0; i < 3; i++) {
    pthread_cond_destroy(&pastelero[i]);
  }

  return 0;
}