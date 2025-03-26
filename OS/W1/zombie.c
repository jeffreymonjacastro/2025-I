#include <unistd.h>
#include <stdio.h>

int main() {
  if (fork() == 0) {
    printf("Child process\n");
  } else {
    printf("Parent process\n");
    sleep(10);
  }
}

