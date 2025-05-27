#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define  N 5 


pthread_mutex_t lock;
pthread_cond_t solicitud;
pthread_mutex_t solicitudes;

int cnt_aterrizaje =0;
int pista_in_use = 0;

int is_there_more_urgent(int id){
    if(cnt_aterrizaje>0) {
        printf("Avión %d: Hay %d aviones esperando para aterrizar (más urgente)\n", id, cnt_aterrizaje);
        return 1;
    }
    printf("Avión %d: No hay aviones esperando para aterrizar\n", id);
    return 0;
}

void solicitar_aterrizaje(int id, int status){
    
    pthread_mutex_lock(&lock);
    
    printf("Avión %d: Solicitando aterrizaje, estado de pista: %s\n", id, pista_in_use ? "OCUPADA" : "LIBRE");
    
    while(pista_in_use==1){
        printf("Avión %d: Esperando que la pista se libere para aterrizar\n", id);
        pthread_cond_wait(&solicitud, &lock);
        printf("Avión %d: Despertado, comprobando pista de nuevo\n", id);
    }
    
    // Aqui solo aterriza un avion
    pista_in_use = 1;
    printf("Avión %d: ¡Pista asignada para aterrizaje!\n", id);
    
    int tiempo_aterrizaje = 2; 
    printf("Avión %d: Aterrizando (tomará %d segundos)...\n", id, tiempo_aterrizaje);
    sleep(tiempo_aterrizaje); // Aterriza 
    
    printf("Avión %d: Aterrizaje completado\n", id);
    cnt_aterrizaje--;
    pista_in_use = 0;
    printf("Avión %d: Liberando pista y notificando a otros aviones\n", id);
    pthread_cond_broadcast(&solicitud);
    
    pthread_mutex_unlock(&lock);
    
}

void solicitar_despegue(int id , int status){
    pthread_mutex_lock(&lock);
    
    printf("Avión %d: Solicitando despegue, estado de pista: %s\n", id, pista_in_use ? "OCUPADA" : "LIBRE");
    
    while(pista_in_use==1 || is_there_more_urgent(id)==1){
        printf("Avión %d: Esperando pista libre o prioridad de aterrizaje\n", id);
        pthread_cond_wait(&solicitud, &lock);
        printf("Avión %d: Despertado, comprobando condiciones de nuevo\n", id);
    };
    
    pista_in_use = 1;
    printf("Avión %d: ¡Pista asignada para despegue!\n", id);
    
    int tiempo_despegue = 3;
    printf("Avión %d: Despegando (tomará %d segundos)...\n", id, tiempo_despegue);
    sleep(tiempo_despegue);
    
    printf("Avión %d: Despegue completado\n", id);
    pista_in_use = 0;
    printf("Avión %d: Liberando pista y notificando a otros aviones\n", id);
    pthread_cond_broadcast(&solicitud);
    
    pthread_mutex_unlock(&lock);
}

void *plane(void * arg){
    
    int id = *(int*)arg;
    
    int status = rand()%2;
    
    printf("Avión %d: Creado con estado %s\n", id, status == 0 ? "ATERRIZAJE" : "DESPEGUE");
    
    if(status==0){  
        pthread_mutex_lock(&solicitudes);
        cnt_aterrizaje++;
        printf("Avión %d: Incrementando contador de aterrizajes a %d\n", id, cnt_aterrizaje);
        pthread_mutex_unlock(&solicitudes);
        solicitar_aterrizaje(id, status); 
    } else if(status ==1)
        solicitar_despegue(id, status);
}


int main(){
    srand(time(NULL)); // Inicializar generador de números aleatorios
    
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&solicitud, NULL); // Corregido: &solicitud en lugar de &pista_in_use
    printf("Aeropuerto: Iniciando simulación...\n");
    
    pthread_t planes[N];
    
    int ids[N];
    for (int i = 0; i < N; i++){
        ids[i] = i;
        printf("Aeropuerto: Creando avión %d\n", i);
        pthread_create(&planes[i], NULL, plane, (void *) &ids[i]);
        // Pequeña pausa para evitar que todos los aviones se creen al mismo tiempo
        usleep(10000); // 10ms
    }
    
    printf("Aeropuerto: Todos los aviones han sido creados, esperando a que terminen...\n");
    for (int i = 0; i < N; i++){
        pthread_join(planes[i], NULL); // Corregido: planes[i] en lugar de &planes[i]
        printf("Aeropuerto: Avión %d ha completado su operación\n", i);
    }
    
    printf("Aeropuerto: Simulación finalizada, limpiando recursos...\n");
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&solicitud); // Corregido: &solicitud en lugar de &pista_in_use
    
    printf("Aeropuerto: Programa terminado correctamente\n");
    return 0;
}

