#include <mpi.h>
#include <stdio.h>

int length, rank;
char hostname[MPI_MAX_PROCESSOR_NAME];

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  MPI_Get_processor_name(hostname, &length);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	  printf("Hello world from host %s (rank %d)!\n", hostname, rank);
  
	  MPI_Finalize();
  return 0;
}

