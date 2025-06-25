#include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <time.h>  

    #define N 15
    #define limit 5

    pthread_mutex_t lock;
    pthread_cond_t sema[4];
    int current_dir = -1;
    int waiting[4] = {0};  
    int active[4] = {0};

    void system_status(int id) {
        printf("System status ...\n");
        for (int i = 0; i < 4; i++) {
            printf("Car_dir : %d  , actives : %d , waiting : %d \n", i, active[i], waiting[i]);
        }
    }

    void enter_(int dir, int id) {
        pthread_mutex_lock(&lock);

        if (current_dir == -1) {
            current_dir = dir;
        }

        waiting[dir]++;  
        while (current_dir != dir) {
            pthread_cond_wait(&sema[dir], &lock);
        }
        waiting[dir]--;  
        active[dir]++;
        pthread_mutex_unlock(&lock);
    }

    void exit_(int dir, int id) {
        pthread_mutex_lock(&lock);

        if (active[dir] == limit || waiting[dir] == 0) {  
            for (int i = (dir + 1) % 4; i != dir; i = (i + 1) % 4) {
                if (waiting[i] > 0) {  
                    current_dir = i;
                    //printf("NEXT DIR ASSIGNED %d...\n", i);
                    //system_status(id);
                    break;
                }
            }
        }

        active[dir]--;

        if (active[dir] == 0) {
            pthread_cond_broadcast(&sema[current_dir]);
        }

        pthread_mutex_unlock(&lock);  
    }

    void* car(void* arg) {
        int dir = rand() % 4;
        int id = *(int*)arg;
        printf("car id : %d on direction : %d  enters dir...\n", id, dir);
        enter_(dir, id);
        sleep(1);
        exit_(dir, id);
        printf("car id : %d on direction : %d  exits dir...\n", id, dir);
        return NULL;  
    }

    int main() {
        srand(time(NULL));
        pthread_t th[N];
        int idx[N];
        pthread_mutex_init(&lock, NULL);
        
        for (int i = 0; i < 4; i++) {
            pthread_cond_init(&sema[i], NULL);  
        }

        for (int i = 0; i < N; i++) {
            idx[i] = i;
            pthread_create(&th[i], NULL, &car, (void*)&idx[i]);
        }

        for (int i = 0; i < N; i++) {
            pthread_join(th[i], NULL);  
        }

        
        pthread_mutex_destroy(&lock);
        for (int i = 0; i < 4; i++) {
            pthread_cond_destroy(&sema[i]);
        }

        return 0;
    }