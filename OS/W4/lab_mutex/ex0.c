/* 
 * Example 0: FUTEX_WAIT
 */ 
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <linux/futex.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define ACQUIRED 0
#define FREE 1

int futex_addr;

/* Return -1 if futex has not blocked, 
            0 otherwise  */
int futex_wait(void *addr, int val1) {
  return syscall(SYS_futex, addr, FUTEX_WAIT, 
                 val1, NULL, NULL, 0);
}

int main() {
  int r;

  futex_addr = ACQUIRED;  
  
  /* Will wait while futex value is the same as val1 */
  r = futex_wait(&futex_addr, FREE);
  printf ("r: %d\n", r);

  return 0; 
}
