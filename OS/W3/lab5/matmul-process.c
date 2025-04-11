#include <stdio.h>
#include <stdlib.h>

#define N 1024	// number of rows/columns
int a[N][N], b[N][N], c[N][N];

int main(void)
{
	int i, j, k;

	// Initialize the matrices 'a' and 'b'
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = 2;
			b[i][j] = 3;
		}
	}

	// Matrix multiplication: c = a*b
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			for (k = 0; k < N; k++)
				c[i][j] = c[i][j] + a[i][k] * b[k][j];

#if 0
	printf("The resultant matrix is:\n");
	for (i = 0; i < N; i++) {
		printf (" [row %3d] ", i+1);
		for (j = 0; j < N; j++)
			printf("%d ", c[i][j]);
		printf(" [row %3d]\n ", i+1);
	}
#endif
}
