#include <stdio.h>
#include <pthread.h>

#define N_THREADS 2
#define BIG 10000000L
int counter = 0;
volatile int interest[N_THREADS] = {0};
volatile int order = 0;



void* increment_counter(void* args){
    int me = *((int*)args);
    int other = 1 - me;
    for(int i = 0 ; i < BIG ; ++i){
        __atomic_fetch_add(&interest[me], 1, __ATOMIC_SEQ_CST); //interest[me] = 1
        __atomic_store(&order,&other, __ATOMIC_SEQ_CST);//order = other
        while(interest[other] && order != me);
        counter++;
        __atomic_fetch_sub(&interest[me], 1, __ATOMIC_SEQ_CST); //interest[me] = 0
    }
    return NULL;
}

int main(void){
    pthread_t thread[N_THREADS];
    int ids[N_THREADS];
    for(int i = 0 ; i < N_THREADS ; i++) ids[i] = i;
    
    for(int i = 0 ; i < N_THREADS ; i++){ 
        pthread_create(&thread[i],NULL,increment_counter,&ids[i]);
    }
    for(int i = 0 ; i < N_THREADS ; i++){
        pthread_join(thread[i],NULL);
    }
    printf("Counter : %d\n",counter);

    return 0;
}