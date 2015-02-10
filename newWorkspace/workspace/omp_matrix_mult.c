#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 100

int main(int argc, char *argv[]) {
	omp_set_num_threads(16); //set number of threads here
	int i, j, k, l;
	double sum;
	double start, end; // used for timing  double A[N][N], B[N][N], C[N][N];
	double total = 0;

	double A[N][N], B[N][N], C[N][N];

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			A[i][j] = j * 1;
			B[i][j] = i * j + 2;
			C[i][j] = j - i * 2;
		}
	}

	for (l = 0; l < 10; l++) {
		start = omp_get_wtime(); //start time measurement
#pragma omp parallel for shared(A, B, C) private(i,j,k)
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				sum = 0;
				for (k = 0; k < N; k++) {
					sum += A[i][k] * B[k][j];
				}
				C[i][j] = sum;
			}
		}

		end = omp_get_wtime(); //end time measurement
		printf("Time of computation: %f seconds\n", end - start);
		total += end - start;
	}

	printf("Average computation time: %f seconds\n", total / 10);
	return (0);
}

