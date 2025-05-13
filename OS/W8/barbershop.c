#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5
#define M 10

phtread_mutex_t baber_chair ;
pthread_mutex_t client_request;
pthread_mutex_t service_protection;
pthread_cond_t haircut;
phtread_cond_t next_customer;
sem_t waiting_room;

volatile int in_service = 1;


void *barber(void * arg){

  
  pthread_mutex_lock(&baber_chair);
  while(in_service==1)
  {
   pthread_cond_wait(&next_customer , &barber_chair); 
  }

  sleep(2);
  printf("Customer : %d is under service...",idx);

  if (in_service = 0){
    in_service=1;
    pthread_cond_signal(&haircut);
  }

  sem_post(&waiting_room);
  pthread_mutex_unlock(&barber);
}

void *customer(void * arg){
  int idx = *(int* ) arg;

  sem_wait(&waiting_room);

  pthread_mutex_lock(&service_protection);

  while(in_service==0){
    pthread_cond_wait(&haircut , &service_protection);
  }

  phtead_mutex_lock(&client_request);
  in_service = 0;
  pthread_mutex_unlock(&client_request);

  pthread_cond_signal(&next_customer);



  pthread_unlock(&service_protection);
  sem_post(&waiting_room);

}


int main(){

  phtread_t customer_[M] , barberillo ;
  int idx[M];
  pthread_mutex_init(&barber_chair);
  sem_init(&waiting_room , 0 , N );

  pthread_create(&barberillo , NULL , &barber, NULL );
  
  for (int i = 0; i < M; i++)
  {
    idx[i]=i;
    phtread_create(&customer_[i] , NULL , &customer , (void*)&idx);
  }

  for (int i = 0; i < M; i++)
  {
    pthread_join(customer_[i] , NULL);
  }
  
  
  




}