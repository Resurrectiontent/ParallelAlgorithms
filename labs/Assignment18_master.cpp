#include "mpi.h"
#include "stdio.h"

int main(int argc, char **argv)
{
	int size, rank1, rank2, inttersize;
	MPI_Status status;
	MPI_Comm intercomm;
	char slave[10] = "./A18s.o";
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_spawn(slave, MPI_ARGV_NULL, 3, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);
	MPI_Recv(&rank1, 1, MPI_INT, 0, 0, intercomm, &status);
    MPI_Recv(&rank2, 1, MPI_INT, 1, 1, intercomm, &status);
    MPI_Recv(&inttersize, 1, MPI_INT, 2, 0, intercomm, &status);
	//Display "Slaves rank1 and rank2 are working", instead of the words rank1 and rank2 their values should be displayed.
    printf("Slaves %d and %d are working. Watcher says that %d in total\n", rank1, rank2, inttersize);
	MPI_Finalize();
	return 0;
}