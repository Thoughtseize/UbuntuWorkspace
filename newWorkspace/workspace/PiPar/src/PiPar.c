#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
	int rank, size, i, j, listener = 1;
	int totalSent = 0;
	double seed[200], package[2], x, y, dist, pi, recvPackage[2], inside = 0;
	double totalInside = 0;
	double elapsed_time;
	struct timeval tv1, tv2;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Take a time stamp
	gettimeofday(&tv1, NULL);

	srand(time(NULL));
	//initial scatter to the slaves
	if (rank == 0) {

		for (i = 1; i < size; i++) {
			for (j = 0; j < 200; j++) {
				seed[j] = (double) rand() / RAND_MAX;
			}
			MPI_Send(seed, 200, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			totalSent += 100;
		}
	}

	//continue gathering results
	if (rank == 0) {
		//collect finished slave processes and give them work after they are done
		while (totalSent < 10000) {
			MPI_Recv(recvPackage, 2, MPI_DOUBLE, MPI_ANY_SOURCE,
			MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			totalSent += 100;
			totalInside += recvPackage[0];
			for (j = 0; j < 200; j++) {
				seed[j] = (double) rand() / RAND_MAX;
			}
			MPI_Send(seed, 200, MPI_DOUBLE, recvPackage[1], 0,
			MPI_COMM_WORLD);

		}

		//once all of the data has been sent, finish collecting the rest of the workpools
		for (i = 1; i < size; i++) {
			MPI_Recv(recvPackage, 2, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,
			MPI_COMM_WORLD, &status);
			totalInside += recvPackage[0];
			//dummy send to tell them to break
			seed[0] = 100;
			MPI_Send(seed, 200, MPI_DOUBLE, recvPackage[1], 0,
			MPI_COMM_WORLD);
		}
	} else {
		while (listener == 1) {
			MPI_Recv(seed, 200, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
					&status);
			if (seed[0] == 100) {
				break;
			}
			//compute results in the slave processes
			for (i = 0; i < 200; i += 2) {
				x = seed[i];
				y = seed[i + 1];

				dist = x * x + y * y;

				if (dist <= 1) {
					inside++;
				}
			}
			package[0] = inside;
			package[1] = rank;
			inside = 0; //reset for the next workpool iteration
			MPI_Send(package, 2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}
	}

	if (rank == 0) {
		pi = (totalInside / 10000) * 4;
		gettimeofday(&tv2, NULL);
		elapsed_time = (tv2.tv_sec - tv1.tv_sec)
				+ ((tv2.tv_usec - tv1.tv_usec) / 1000000.0);
		printf("elapsed_time=\t%lf (seconds)\n", elapsed_time);
		printf("%f\n", pi);
		printf("difference = %f\n", (pi - 3.141593));
	}



	/* shut down MPI */
	MPI_Finalize();

	return 0;
}
