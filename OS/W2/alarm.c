#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void myHandler(int signum) {
  printf("Jamcy\n");
  exit(0);
}

int main() {
  struct sigaction sAction;

  sAction.sa_flags = 0;
  sAction.sa_handler = myHandler;
  sigemptyset(&sAction.sa_mask);

  int iRet = sigaction(SIGALRM, &sAction, NULL);
  assert(iRet == 0);

  printf("Esperando 3 segundos para recibir SIGALRM...\n");
  alarm(3);

  while (1) {
    sleep(1);
  }

  return 0;
}
