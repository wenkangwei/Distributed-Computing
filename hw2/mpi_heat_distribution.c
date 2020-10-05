#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv){

int rank, size;
MPI_Status status;

//initialize MPI environment
MPI_Initialize();
MPI_Comm_Rank(MPI_COMM_WORLD, &rank);
MPI_Comm_Size(MPI_COMM_WORLD,&size);






return 0;
}
