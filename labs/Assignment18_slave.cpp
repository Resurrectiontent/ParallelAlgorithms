#include "mpi.h"
int main(int argc, char **argv)
{
	int rank, size;
	MPI_Comm intercomm;
	MPI_Init(&argc, &argv);
	MPI_Comm_get_parent(&intercomm);
    MPI_Comm_size(intercomm, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 2)
	    MPI_Send(&rank, 1, MPI_INT, 0, rank, intercomm);
    else
        MPI_Send(&size, 1, MPI_INT, 0, 0, intercomm);
	MPI_Finalize();
	return 0;
}