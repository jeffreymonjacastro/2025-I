#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void myHandler(int signum) {
  printf("In myHandler with argument %d\n", signum);
}

int main(int argc, char* argv) {
  int iRet;
  struct sigaction sAction;

  sAction.sa_flags = 0;
  sAction.sa_handler = myHandler;
  sigemptyset(&sAction.sa_mask);

  for (int i = 1; i < 65; i++) {
    iRet = sigaction(i, &sAction, NULL);
    if (iRet != 0) {
      printf("Error in sigaction for signal %d\n", i);
    } else {
      printf("sigaction for signal %d succeeded\n", i);
    }
  }

  while (1) {
    sleep(1);
  }

  return 0;
}
