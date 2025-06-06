#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1024 // number of rows/columns
int a[N][N], b[N][N], c[N][N];

#define NCORES 6 // # CPU cores; adjust this value for your system!

/* Each thread will run this function, working on a different row of
 * the matrix.
 * */
void *threadfun(void *data_arg) {
  int *p = (int *)data_arg;
  int i, j, k;

  for (i = *p; i < (*p + (N / NCORES)); i++)
    for (j = 0; j < N; j++)
      for (k = 0; k < N; k++)
        c[i][j] = c[i][j] + a[i][k] * b[k][j];

  pthread_exit((void *)0);
}

int main(void) {
  int i, j, row[NCORES];
  pthread_t tid[NCORES];

  // Initialize the matrices 'a' and 'b'
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = 2;
      b[i][j] = 3;
    }
  }

  /* Create a worker thread for each CPU core; each thread
   * will therefore perform a part of the computation!
   * */
  for (i = 0; i < NCORES; i++) {
    row[i] = i * (N / NCORES);
    pthread_create(&tid[i], NULL, threadfun, &row[i]);
  }

  // Wait until all other threads complete
  for (i = 0; i < NCORES; i++)
    pthread_join(tid[i], NULL);

#if 0
	printf("The resultant matrix is:\n");
	for (i = 0; i < N; i++) {
		printf(" [row %3d] ", i+1);
		for (j = 0; j < N; j++) {
			printf("%d ", c[i][j]);
		}
		printf(" [row %3d]\n", i+1);
	}
#endif
  pthread_exit((void *)0);
}
