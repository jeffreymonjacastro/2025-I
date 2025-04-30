#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FILENAME "archivito.txt"

int fd;

static void myHandler(int signum) {
  printf("Deleting file %s\n", FILENAME);
  // Close the file and then remove it
  close(fd);
  remove(FILENAME);
  exit(0);
}

int main() {
  fd = open(FILENAME, O_CREAT);

  int iRet;
  struct sigaction sAction;
  sAction.sa_flags = 0;
  sAction.sa_handler = myHandler;

  sigemptyset(&sAction.sa_mask);

  iRet = sigaction(SIGINT, &sAction, NULL);
  assert(iRet == 0);

  printf("Working with file %s\n", FILENAME);
  while (1) {
    sleep(1);
  }

  return 0;
}
