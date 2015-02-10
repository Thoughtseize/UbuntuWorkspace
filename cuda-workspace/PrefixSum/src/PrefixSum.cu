#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

__global__ void prefixSum(int *a, int N) {
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
	int i;
	extern __shared__ int bufferA[];

	if (tid < N) {
		for (i = 0; i < log2((double) N); i++) {
			if (tid >= powf(2, i) && (tid + (int) powf(2, i) < N)) {
				bufferA[tid] = a[tid] + a[tid + (int) powf(2, i)];
			}
			__syncthreads();

			a[tid] = bufferA[tid];
			__syncthreads();
		}
	}
}

int main(void) {

	int N, T, B, repeat;
	repeat = 1;

	while (repeat == 1) {

		printf("Enter size of matrices: ");
		scanf("%d", &N);

		while (N <= 0) {
			printf(
					"Size of matrices must be greater than 0. Enter a valid size of matrices: ");

			scanf("%d", &N);
		}

		printf("Enter number of threads in a block: ");
		scanf("%d", &T);

		while (T <= 0) {
			printf(
					"Number of threads must be greater than 0. Enter number of threads in a block: ");
			scanf("%d", &T);

		}

		while (T > 1024) {
			printf(
					"Number of threads must not exceed the device bandwidth. Enter number of threads in a block: ");
			scanf("%d", &T);
		}

		printf("Enter number of blocks in a grid: ");
		scanf("%d", &B);

		while (B <= 0) {

			printf(
					"Number of blocks must be greater than 0. Enter number of blocks in a grid: ");
			scanf("%d", &B);
		}

		while (B > 65535) {
			printf(
					"Number of blocks must not exceed the device bandwidth. Enter number of blocks in a grid: ");
			scanf("%d", &B);
		}

		int *a, *deviceA;
		int *dev_a;
		int i, j;
		int ssd = 0;

		cudaEvent_t start, stop;
		float elapsedTime;
		cudaEventCreate(&start);
		cudaEventCreate(&stop);

		cudaMalloc((void**) &dev_a, N * sizeof(int));

		a = (int *) malloc(N * sizeof(int));
		deviceA = (int *) malloc(N * sizeof(int));

		srand(time(NULL));

//loop will generate the matrix with random integers

		for (i = 0; i < N; i++) {
			a[i] = (int) rand();
			deviceA[i] = a[i];
		}
		/*******************begin host code*****************************/
		clock_t begin, end;
		begin = clock();

		for (j = 0; i < (log(N) / log(2)); j++) {
			for (i = pow(2, j); i < N; i++) {
				if (i + (int) pow(2, j) < N)
					a[i] = a[i] + a[i + (int) pow(2, j)];
			}
		}

		end = clock();

		printf("It took %f seconds for the host to do the matrix operation.\n",
				(float) (end - begin) / (CLOCKS_PER_SEC));

		/*******************end host code*****************************/

		/*******************begin device code*****************************/

		cudaMemcpy(dev_a, deviceA, N * sizeof(int), cudaMemcpyHostToDevice);

		dim3 grid(B, B);
		dim3 block(T, T);

		cudaEventRecord(start, 0);

		prefixSum<<<grid, block>>>(dev_a, N);
		cudaThreadSynchronize();
		cudaEventRecord(stop, 0);
		cudaEventSynchronize(stop);

		cudaMemcpy(deviceA, dev_a, N * sizeof(int),
				cudaMemcpyDeviceToHost);

		cudaEventElapsedTime(&elapsedTime, start, stop);

		printf(
				"It took %f seconds for the device to do the matrix operation.\n",
				(elapsedTime / 1000));

		for (i = 0; i < N; i++) {
			ssd += deviceA[i] - a[i];
		}

		printf("The sum of square difference is %d.\n", ssd);
		printf("The speedup factor is %f.\n",
				((float) (end - begin) / (CLOCKS_PER_SEC))
						/ (elapsedTime / 1000));

		cudaEventDestroy(start);
		cudaEventDestroy(stop);

		free(a);
		free(deviceA);
		cudaFree(dev_a);
		/*******************end device code*****************************/

		printf("Enter 1 to continue: ");
		scanf("%d", &repeat);
	}
	return 0;
}
