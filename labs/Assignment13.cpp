#include <cstdlib>
#include "iostream"
#include "mpi.h"
#define MAT_SIDE_LEN 500
using namespace std;

int** randomMatrix(int size, bool smallInts = false)
{
    int** a = new int*[size];
    for (int i = 0; i < size; ++i)
    {
        a[i] = new int[size];
        for (int j = 0; j < size; ++j)
        {
            a[i][j] = smallInts ? rand() % 10 : rand();
        }
    }

    return a;
}

int** zeroMatrix(int size)
{
    int** a = new int*[size];
    for (int i = 0; i < size; ++i)
    {
        a[i] = new int[size];
        for (int j = 0; j < size; ++j)
        {
            a[i][j] = 0;
        }
    }

    return a;
}

void freeMatrix(int** m, int size)
{
    for (int i = 0; i < size; ++i)
    {
        delete(m[i]);
    }

    delete(m);
}

void dotProduct(int** m1, int** m2, int** result, int size)
{
    for (int i = 0; i < size; ++i) {
        for (int k = 0; k < size; ++k) {
            for (int j = 0; j < size; ++j) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

int main(int argc, char **argv)
{
    int size, rank, prev, next;
    double startTime, endTime;
    MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    prev = rank == 0 ? MPI_PROC_NULL : rank-1;
    next = rank == size - 1 ? MPI_PROC_NULL : rank + 1;
    int **m1 = randomMatrix(MAT_SIDE_LEN);
    int **m2 = randomMatrix(MAT_SIDE_LEN);
    int **res = zeroMatrix(MAT_SIDE_LEN);
//matrix initialization by each process
	MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization
// start timing for each process
    startTime = MPI_Wtime();
// matrix multiplication
    dotProduct(m1, m2, res, MAT_SIDE_LEN);
// end of timing
    endTime = MPI_Wtime();
// output the execution time of matrix multiplication at each process
    double time[2] = {(double)rank, endTime - startTime};
    double bestTime[2];
    if (rank != 0)
        MPI_Recv(&bestTime, 2, MPI_DOUBLE, prev, 0, MPI_COMM_WORLD, &status);
    else {
        bestTime[0] = time[0];
        bestTime[1] = time[1];
     }

    if (time[1] < bestTime[1]){
        bestTime[0] = time[0];
        bestTime[1] = time[1];
    }
    cout << "P" << rank << ": " << time[1] << ",\t curr best: [" << (int)bestTime[0] << "]: " << bestTime[1] << endl;

    MPI_Send(&bestTime, 2, MPI_DOUBLE, next, 0, MPI_COMM_WORLD);
    freeMatrix(m1, MAT_SIDE_LEN);
    freeMatrix(m2, MAT_SIDE_LEN);
    freeMatrix(res, MAT_SIDE_LEN);
	MPI_Finalize();
    return 0;
}