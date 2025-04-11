#include <pthread.h>
#include <stdio.h>

#define BIG 100000000L

long int counter = 0;

void *increase_counter(void *args) {
  for (long int i = 0; i < BIG; i++) {
    counter++;
  }
}

int main() {
  pthread_t a, b;

  pthread_create(&a, NULL, increase_counter, NULL);
  pthread_create(&b, NULL, increase_counter, NULL);

  pthread_join(a, NULL);
  pthread_join(b, NULL);

  printf("Counter: %ld\n", counter);
  return 0;
}