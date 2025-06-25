#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// pls work

#define N 5

#define C 8
#define G 4
#define S 2

typedef struct {
    // current amount
    int cpu_cnt;
    int gpu_cnt;
    int ssd_cnt;

    pthread_cond_t cpu_free;
    pthread_cond_t gpu_free;
    pthread_cond_t ssd_free;

    pthread_mutex_t lock;
} monitor;

void monitor_init(monitor* m, int cpu_tot, int gpu_tot, int ssd_tot) {
    m->cpu_cnt = cpu_tot;
    m->gpu_cnt = gpu_tot;
    m->ssd_cnt = ssd_tot;

    pthread_mutex_init(&m->lock, NULL);

    pthread_cond_init(&m->cpu_free, NULL);
    pthread_cond_init(&m->gpu_free, NULL);
    pthread_cond_init(&m->ssd_free, NULL);
}

void monitor_destroy(monitor* m) {
    pthread_mutex_destroy(&m->lock);

    pthread_cond_destroy(&m->cpu_free);
    pthread_cond_destroy(&m->gpu_free);
    pthread_cond_destroy(&m->ssd_free);
}

typedef struct {
    int cpu_req;
    int gpu_req;
    int ssd_req;
} User;

pthread_t clients[N];
User users[N];

int ind[N];

monitor* m;

void user (monitor* m, int id) {
    pthread_mutex_lock(&m->lock);

    while (users[id].cpu_req > m->cpu_cnt
           || users[id].gpu_req > m->gpu_cnt
           || users[id].ssd_req > m->ssd_cnt
    ) {
        printf("User %d waiting for HW resources\n\n", id);
        printf("           CPU | GPU | SSD\n");
        printf("Available: %3d | %3d | %3d\n", m->cpu_cnt, m->gpu_cnt, m->ssd_cnt);
        printf("Requested: %3d | %3d | %3d\n\n", users[id].cpu_req, users[id].gpu_req, users[id].ssd_req);

        if (users[id].cpu_req > m->cpu_cnt) {
            pthread_cond_wait(&m->cpu_free, &m->lock);
        }
        if (users[id].gpu_req > m->gpu_cnt) {
            pthread_cond_wait(&m->gpu_free, &m->lock);
        }
        if (users[id].ssd_req > m->ssd_cnt) {
            pthread_cond_wait(&m->ssd_free, &m->lock);
        }
    }

    printf("User %d consuming HW resources\n", id);
    printf("           CPU | GPU | SSD\n");
    printf("Available: %3d | %3d | %3d\n", m->cpu_cnt, m->gpu_cnt, m->ssd_cnt);
    printf("Requested: %3d | %3d | %3d\n\n", users[id].cpu_req, users[id].gpu_req, users[id].ssd_req);

    m->cpu_cnt -= users[id].cpu_req;
    m->gpu_cnt -= users[id].gpu_req;
    m->ssd_cnt -= users[id].ssd_req;

    pthread_mutex_unlock(&m->lock);

    sleep(1);

    pthread_mutex_lock(&m->lock);

    if (users[id].cpu_req) {
        m->cpu_cnt += users[id].cpu_req;
        pthread_cond_broadcast(&m->cpu_free);
    }
    if (users[id].gpu_req) {
        m->gpu_cnt += users[id].gpu_req;
        pthread_cond_broadcast(&m->gpu_free);
    }
    if (users[id].ssd_req) {
        m->ssd_cnt += users[id].ssd_req;
        pthread_cond_broadcast(&m->ssd_free);
    }

    printf("User %d satisfied :)\n\n", id);

    pthread_mutex_unlock(&m->lock);
}

void* client(void* vid) {
    int id = *((int*)vid);
    // while (1) {
        users[id].cpu_req = rand() % C;
        users[id].gpu_req = rand() % G;
        users[id].ssd_req = rand() % S;
        user(m, id);
        sleep(1);
    // }
}

int main (int argc, char* argv[]) {
    srand(time(NULL));

    m = (monitor*)malloc(sizeof(monitor));

    monitor_init(m, C, G, S);

    for (int i=0; i<N; ++i) {
        ind[i] = i;
        pthread_create(clients+i, NULL, client, (void*)(ind+i));
    }

    for (int i=0; i<N; ++i)
        pthread_join(clients[i], NULL);

    monitor_destroy(m);

    free(m);

    return 0;
}
