#define N 512

#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "mpi.h"

void print_results(char *prompt, double a[N][N]);

int main(int argc, char *argv[]) {
	int i, j, k, rank, size, blksz, error = 0;
	double a[N][N], b[N][N];
	char *usage = "Usage: %s file\n";
	FILE *fd;
	double elapsed_time;
	struct timeval tv1, tv2;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	blksz = (N * N) / size;
	double recA[N / size][N], c[N / size][N];
	double recC[N][N];

	//only 1 process needs to do file I/O
	if (rank == 0) {
		if (argc < 2) {
			fprintf(stderr, usage, argv[0]);
			error = -1;
		}

		if ((fd = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr, "%s: Cannot open file %s for reading.\n", argv[0],
					argv[1]);
			fprintf(stderr, usage, argv[0]);
			error = -1;
		}
	}
	//broadcast the error to all processes, then check if it's not 0.
	//if it's not 0, close the MPI function with MPI_Finalize and return from the function
	//(so the function stops processing on every process)
	MPI_Bcast(&error, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (error != 0) {
		MPI_Finalize();
		return 0;
	}

// Read input from file for matrices a and b.
// The I/O is not timed because this I/O needs
// to be done regardless of whether this program
// is run sequentially on one processor or in
// parallel on many processors. Therefore, it is
// irrelevant when considering speedup.

	//only need to read the input once
	if (rank == 0) {
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
				fscanf(fd, "%lf", &a[i][j]);

		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
				fscanf(fd, "%lf", &b[i][j]);
	}
	//barrier before the time stamp so the processes synchronize before calculation
	MPI_Barrier(MPI_COMM_WORLD);

// Take a time stamp
	gettimeofday(&tv1, NULL);

	MPI_Scatter(a, blksz, MPI_DOUBLE, recA, blksz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(b, (N * N), MPI_DOUBLE, 0, MPI_COMM_WORLD);

	for (i = 0; i < (N / size); i++) {
		for (j = 0; j < N; j++) {
			c[i][j] = 0;
			for (k = 0; k < N; k++) {
				c[i][j] = c[i][j] + recA[i][k] * b[k][j];
			}
		}
	}

	MPI_Gather(c, blksz, MPI_DOUBLE, recC, blksz, MPI_DOUBLE, 0, MPI_COMM_WORLD);
// Take a time stamp. This won't happen until after the master
// process has gathered all the input from the other processes.
	if (rank == 0) {
		gettimeofday(&tv2, NULL);

		elapsed_time = (tv2.tv_sec - tv1.tv_sec)
				+ ((tv2.tv_usec - tv1.tv_usec) / 1000000.0);
		printf("elapsed_time=\t%lf (seconds)\n", elapsed_time);

		// print results
		print_results("C = ", recC);
	}

	MPI_Finalize();
	return 0;
}

void print_results(char *prompt, double a[N][N]) {
	int i, j;

	printf("\n\n%s\n", prompt);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf(" %.2lf", a[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}
