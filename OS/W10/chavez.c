#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N 10
#define H 5

int tarro = 0;

sem_t bear_eating;
sem_t bee_filling;
sem_t wait_for_bear;


void * bee(){

    while(1){
        sem_wait(&wait_for_bear);

    sem_wait(&bee_filling);

    tarro++;

    printf("se esta llenando el tarro...\n");

    if(tarro==H)
    {  printf("tarro lleno le toca al oso comer...\n");
        sem_post(&bear_eating);
        sem_wait(&wait_for_bear);
    }


    

    sem_post(&wait_for_bear);
    }
    

}

void * bear(){
    while(1){
    sem_wait(&bear_eating);

    tarro = 0;

    printf("El oso esta comiendo...\n");
    sleep(1);

    for (int i = 0; i < H; i++)
    {
        sem_post(&bee_filling);
    }

    sem_post(&wait_for_bear);
    
   
}
    

}



int main(){
pthread_t bees[N];
pthread_t the_bear;
int idx[N];

sem_init(&bear_eating, 0 , 0);
sem_init(&bee_filling , 0 , H);
sem_init(&wait_for_bear , 0 , 1);

pthread_create(&the_bear , NULL , &bear , NULL);

for(int i=0 ; i<N ; i++){
    idx[i] = i ;
    pthread_create(&bees[i] , NULL , &bee , (void*)&idx);

}

pthread_join(the_bear , NULL);
for (int i = 0; i < N; i++)
{
    pthread_join(bees[i], NULL);
}
pthread_exit(0);
}