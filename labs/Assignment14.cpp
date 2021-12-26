#include <stdio.h>
#include "mpi.h"
#define n 1000

void smod5(void *a, void *b, int *l, MPI_Datatype *type) {
	int i;
	for (i = 0; i < *l; i++)
		((int*)b)[i] = (((int*)a)[i] + ((int*)b)[i]) % 5;
}

void max(void *a, void *b, int *l, MPI_Datatype *type){
    for (int i = 0; i < *l; i++)
        ((int*)b)[i] = ((int*)a)[i] > ((int*)b)[i]
                ? ((int*)a)[i]
                : ((int*)b)[i];
}

int main(int argc, char **argv)
{
	int rank, size, i, a[n], b[n];
    MPI_Op op;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for (i = 0; i < n; i++) a[i] = i + rank + 1;
	printf("process %d a[0] = %d\n", rank, a[0]);
	MPI_Op_create(&max, 1, &op);

    double startTime = MPI_Wtime();
	MPI_Reduce(a, b, n, MPI_INT, op , 0, MPI_COMM_WORLD);
    if (rank == 0) printf("for p1 custom max executed in %f\n", MPI_Wtime() - startTime);
	MPI_Op_free(&op);
	if (rank == 0) printf("b[0] = %d\n", b[0]);

    startTime = MPI_Wtime();
    MPI_Reduce(a, b, n, MPI_INT, MPI_MAX , 0, MPI_COMM_WORLD);
    if (rank == 0) printf("for p1 MPI_MAX executed in %f\n", MPI_Wtime() - startTime);
    if (rank == 0) printf("b[0] = %d\n", b[0]);

	MPI_Finalize();
}