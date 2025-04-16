#include <stdio.h>

#define BIG 100000000L

long int counter = 0;

void increase_counter() {
  for (long int i = 0; i < BIG; i++) {
    counter++;
  }
}

int main() {
  increase_counter();
  increase_counter();

  printf("Counter: %ld\n", counter);
  return 0;
}