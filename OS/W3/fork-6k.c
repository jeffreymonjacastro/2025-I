#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NFORKS 60000

void do_nothing ()
{
	unsigned long f = 0xb00da;
}

int main ()
{
	int pid, j, status;

	for (j = 0; j < NFORKS; ++j) {
		switch (pid = fork()) {
			case -1:
				printf("fork failed! [%d]\n", pid);
				break;
			case 0:
				do_nothing();
				exit(EXIT_SUCCESS);

			default:
				waitpid(pid, &status, 0);
		}
	}

	exit(EXIT_SUCCESS);
}
