#ifndef SUZAKU_H
#define SUZAKU_H
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define PRINTINDEX(x) printf(" %.2lf", x)
#define MPI_START(p,rank,argc, argv)                                    \
        MPI_Init(argc, argv);                                           \
        MPI_Comm_size(MPI_COMM_WORLD, p);                               \
        MPI_Comm_rank(MPI_COMM_WORLD, rank)

#define MPI_SEND_INDEX_AND_RESULT(array, index, rank, blksize)          \
        MPI_Send(&index, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);          \
        MPI_Send(&array[index], N * blksize, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);

#define N 512

#define BUFFSIZE  10000
struct timeval tv1, tv2;
MPI_Status status;
MPI_Request request[32];
MPI_Status statuses[32];

//Broadcasts a 2 demisional array to the processes
void mpiBroadcastArrayOfDoubles(double *array);
//Manages the work flow of the workers
void masterProcess(double array1[N][N], double array2[N][N], int p, int rank, int blksize);
//Workers receive work and call the compute function provided to get results
void workerProcess(double array1[N][N], double array2[N][N], double array3[N][N], int rank, int blksize);
//Print the results of a matrix of double
void printResults(char *prompt, double array[N][N], int rank);
//Takes the input arguments from the command line and reads the filename to
//read the values into the arrays
void readInputFile(int argc, char *argv[], int *error, double array1[N][N], double array2[N][N]);
//Starts the measurement of execution time by calling timeofday
void startTimer(int rank);
//Stops the measurement of execution time and displays the calculated time
//also using timeofday and terminates MPI
void stopTimer(int rank);
//The function that needs to be defined in the workpool implementation,
//this function is called by the workerProcess function
void compute(double array1[N][N], double array2[N][N], double array3[N][N], int index, int blksize);
//Puts the worker in waiting status to recieve next piece of work
void workerGetRowOfDoubles(double array[N][N], int *index, int rank, int blksize);

#endif