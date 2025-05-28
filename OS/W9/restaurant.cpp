#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#define N 5 // N grupos de clientes
#define M 6 // Cant de tipo de mesas

int mesas[M+1] = {0, 0, 1, 0, 1, 0, 1}; // [0] = 2, [1] = 4, [2] = 6
int waiting[N] = {N+1};
int ticket = 0;

pthread_mutex_t lock;
pthread_cond_t cond_available_mesas;

int min_mesa(int tam_grupo){
    for (int i = tam_grupo; i < M + 1; i++){
        if (mesas[i] > 0) {
            return i;
        }
    }
    return -1;
}

int solicitar_mesa(int tam_grupo, int id_grupo){
    return min_mesa(tam_grupo) == -1;
}

void liberar_mesa(int id_mesa, int id_grupo){
    mesas[id_mesa]++;
    waiting[id_grupo] = N+1;
    printf("Cliente %d: broadcasting a todos los clientes en espera\n", id_grupo);
    pthread_cond_broadcast(&cond_available_mesas);
}

int am_i_min(int id_cliente){
    for (int i = 0; i < N; i++){
        if (waiting[id_cliente] > waiting[i]){
            printf("Cliente %d: no es el mínimo (tiene %d, pero cliente %d tiene %d)\n", 
                  id_cliente, waiting[id_cliente], i, waiting[i]);
            return 0;
        }
    }
    printf("Cliente %d: es el mínimo con ticket %d\n", id_cliente, waiting[id_cliente]);
    return 1;
}

void* cliente(void* args){
    int id = *(int *) args;
    int tam = rand()%10 + 1;
    
    printf("Cliente %d: grupo de tamaño %d solicitando mesa\n", id, tam);
    
    // Si el grupo es mayor al maximo de mesas
    if (tam > M) {
        printf("Cliente %d: tamaño de grupo demasiado grande, cancelando\n", id);
        return NULL;
    }
    
    pthread_mutex_lock(&lock);
    // Asignar orden 
    waiting[id] = ticket++;
    printf("Cliente %d: ticket asignado = %d\n", id, waiting[id]);
    
    // No hay mesas disponibles o no soy el minimo
    while (solicitar_mesa(tam, id) == 1 || am_i_min == 0) {
        printf("Cliente %d: esperando mesa disponible o turno\n", id);
        pthread_cond_wait(&cond_available_mesas, &lock);
        printf("Cliente %d: despierto, comprobando de nuevo\n", id);
    };
    
    // Si hay mesas disponibles mayor o igual a tam, escoger la min
    int min = min_mesa(tam);
    printf("Cliente %d: recibe mesa de tamaño %d\n", id, min);
    
    // Ocupar la mesa
    mesas[min]--;
    printf("Cliente %d: mesa ocupada, quedan %d mesas de tamaño %d\n", id, mesas[min], min);
    pthread_mutex_unlock(&lock);
    
    // comiendo
    int tiempo_comida = rand()%2 + 1;
    printf("Cliente %d: comiendo por %d segundos\n", id, tiempo_comida);
    sleep(tiempo_comida);
    
    // Desocupar la mesa
    pthread_mutex_lock(&lock);
    printf("Cliente %d: liberando mesa de tamaño %d\n", id, min);
    liberar_mesa(min, id);
    printf("Cliente %d: mesa liberada, ahora hay %d mesas de tamaño %d\n", id, mesas[min], min);
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t clientes[N];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond_available_mesas, NULL);
    
    printf("Restaurante abierto!\n");

    int ids[N];
    for (int i = 0; i < N; i++){
        ids[i] = i;
        printf("Creando cliente %d\n", i);
        pthread_create(&clientes[i], NULL, &cliente, (void *) &ids[i]);
    }
    
    for (int i = 0; i < N; i++){
        pthread_join(clientes[i], NULL);
        printf("Cliente %d ha terminado\n", i);
    }
    
    printf("Restaurante cerrado!\n");
    printf("Estado final de las mesas:\n");
    for (int i = 1; i <= M; i++) {
        if (mesas[i] > 0) {
            printf("- %d mesas de tamaño %d\n", mesas[i], i);
        }
    }
}
