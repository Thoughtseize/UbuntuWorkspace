#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 1000
#define T 5000

int main(int argc, char *argv[]) {
	int iteration, i, j;

	//static so it can use the global memory space in order
	//to avoid segfaults
	static double g[N][N][2];
	static double ssd[N][N];
	double ssdTotal = 0;

	int x = 0;
	double elapsed_time;
	struct timeval tv1, tv2;
	/******************sequential version**************************/
//initialize matrices (assuming that room temperature is 20, the same as the walls
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			g[i][j][0] = 20;
			g[i][j][1] = 20;
		}
	}

//initialize "fireplace" to 100C
	for (i = 300; i < 700; i++) {
		g[0][i][0] = 100;
		g[0][i][1] = 100;
	}

	gettimeofday(&tv1, NULL);

//do Jacobi iterations
	for (iteration = 0; iteration < T; iteration++) {
		for (i = 1; i < N - 1; i++) {
			for (j = 1; j < N - 1; j++) {
				if (x == 0) {
					g[i][j][0] = 0.25
							* (g[i - 1][j][1] + g[i + 1][j][1] + g[i][j - 1][1]
									+ g[i][j + 1][1]);
					x = 1;
				} else {
					g[i][j][1] = 0.25
							* (g[i - 1][j][0] + g[i + 1][j][0] + g[i][j - 1][0]
									+ g[i][j + 1][0]);
					x = 0;
				}
			}
		}
	}
	gettimeofday(&tv2, NULL);
	elapsed_time = (tv2.tv_sec - tv1.tv_sec)
			+ ((tv2.tv_usec - tv1.tv_usec) / 1000000.0);
	printf("sequential elapsed_time=\t%lf (seconds)\n", elapsed_time);
	/**************************end sequential version***************/

	/**************************start parallel version***************/
	double start, end;
	static double h[N][N][2];

	//initialize matrices (assuming that room temperature is 20, the same as the walls
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			h[i][j][0] = 20;
			h[i][j][1] = 20;
		}
	}

	//initialize "fireplace" to 100C
	for (i = 300; i < 700; i++) {
		h[0][i][0] = 100;
		h[0][i][1] = 100;
	}

	start = omp_get_wtime();

	//do Jacobi iterations
	for (iteration = 0; iteration < T; iteration++) {
#pragma omp parallel for shared(h) private(i, j, x)
		for (i = 1; i < N - 1; i++) {
			for (j = 1; j < N - 1; j++) {
				if (x == 0) {
					h[i][j][0] = 0.25
							* (h[i - 1][j][1] + h[i + 1][j][1] + h[i][j - 1][1]
									+ h[i][j + 1][1]);
					x = 1;
				} else {
					h[i][j][1] = 0.25
							* (h[i - 1][j][0] + h[i + 1][j][0] + h[i][j - 1][0]
									+ h[i][j + 1][0]);
					x = 0;
				}
			}
		}
	}

	end = omp_get_wtime();
	printf("Parallel Elapsed Time: %f seconds\n", end - start);

	/**************end parallel version******************/

	/*******Calculate sum of square difference and speedup factor********/

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			ssd[i][j] = g[i][j][0] - h[i][j][0];
			ssdTotal += ssd[i][j];
		}
	}

	printf("The speed-up factor is: %f\n", elapsed_time/(end - start));

	/*******End calculations********/


	printf("The sum of square difference is %f.\n", ssdTotal);
	return 0;
}
