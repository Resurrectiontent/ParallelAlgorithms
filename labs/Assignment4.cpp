#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char* argv[]) {
    const int VARIANT_NUMBER = 8;
    double start = MPI_Wtime();
	MPI_Init(&argc, &argv);
	int rank, n, i, message;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == VARIANT_NUMBER-1)
	{
		cout << "Hello from process " << rank << "\n";
		for (i = 0; i < n; i++) {
            if (i == VARIANT_NUMBER-1) continue;

			MPI_Recv(&message, 1, MPI_INT, i,
			         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << "Hello from process " << message + rank << endl;
		}
	}
	else MPI_Send(&rank, 1, MPI_INT,
                  VARIANT_NUMBER-1, 0, MPI_COMM_WORLD);
	MPI_Finalize();
    if (rank == VARIANT_NUMBER-1)
        cout<<"Execution time: "<<MPI_Wtime()-start<<endl;
	return 0;
}