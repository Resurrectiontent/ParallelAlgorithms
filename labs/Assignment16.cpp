#include <cstdlib>
#include "mpi.h"
#include "stdio.h"

int main(int argc, char **argv)
{
	int rank, size, rank1, color, choice;
    MPI_Init(&argc, &argv);
    MPI_Comm comm_revs;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    choice = atoi(argv[1]);
    switch (choice){
        case 2:
            color = rank % 2;
            break;
        case 3:
            color = rank % 3;
            break;
        default:
            color = 1;
            break;
    }

	MPI_Comm_split(MPI_COMM_WORLD, color, size - rank, &comm_revs);
	MPI_Comm_rank(comm_revs, &rank1);
	//Display rank and rank1
    printf("P[%d] -> [%d]\n", rank, rank1);
	MPI_Comm_free(&comm_revs);
	MPI_Finalize();
}