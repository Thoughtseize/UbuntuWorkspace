#include "suzaku.h"
 
void compute(double a[N][N], double b[N][N], double c[N][N], int index, int 
blksize){ 
 
} 
 
int main(int argc, char **argv){ 
 int p, rank; 
 
 MPI_START(&p, &rank, &argc, &argv); 
 
 for(int i = 0; i < 10; i++){ 
 printf("Hello world from process: %i \n", rank); 
 } 
 
 MPI_Finalize(); 
 return 0; 
}
