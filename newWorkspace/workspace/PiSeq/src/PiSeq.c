#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int main(void) {
	int i;
	double x, y, dist, pi, inside = 0;
	double elapsed_time;
	struct timeval tv1, tv2;
	//initialize random seed
	srand(time(NULL));

	// Take a time stamp
	gettimeofday(&tv1, NULL);

	for (i = 0; i < 10000; i++) {
		x = (double) rand() / RAND_MAX;
		y = (double) rand() / RAND_MAX;

		dist = x * x + y * y;

		if (dist <= 1) {
			inside++;
		}
	}

	pi = (inside / 10000) * 4;
	gettimeofday(&tv2, NULL);
	elapsed_time = (tv2.tv_sec - tv1.tv_sec)
			+ ((tv2.tv_usec - tv1.tv_usec) / 1000000.0);
	printf("elapsed_time=\t%lf (seconds)\n", elapsed_time);
	printf("pi = %f\n", pi);
	printf("difference = %f\n", (pi - 3.141593));

	return 0;
}
