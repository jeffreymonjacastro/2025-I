#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void process() {
  printf("Process is running...\n");

  int pid = fork();
  int pid2 = fork();

  if (pid < 0 || pid2 < 0) {
    perror("Fork failed");
    exit(EXIT_FAILURE);
  } 

  // printf("PID: %d, pid: %d, pid2: %d\n", getpid(), pid, pid2);

  if (pid == 0 && pid2 == 0) {
    printf("Child process (PID: %d) created from both forks.\n", getpid());
  } else if (pid > 0 && pid2 == 0) {
    printf("Child process (PID: %d) created from second fork.\n", getpid());
    wait(NULL);
  } else if (pid == 0 && pid2 > 0) {
    printf("Child process (PID: %d) created from first fork.\n", getpid());
    wait(NULL);
  } else {
    printf("Parent process (PID: %d) created two children.\n", getpid());
    wait(NULL);
  }


  printf("Process (PID: %d) completed.\n", getpid());

}

int main() {
  process();
  return 0;
}