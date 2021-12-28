#include "cstdio"
#include "mpi.h"
#include "string"
#include "fstream"
#define FILE "file1.txt"
#define DATA "123456789\n"
#define DATA_REPLICATION_DEGREE 20
using namespace std;

void createFile(string filename, string data, long replicationDegree = 1){
    ofstream outfile;
    outfile.open(filename);
    for(long i = 0; i < replicationDegree; ++i ) {
        outfile << hex << i << (i > 4095 ? "\t" : "\t\t") << data;
    }
    outfile.close();
}

int main(int argc, char **argv)
{
	int rank, size;
    int *buf = new int[1];
    long rBufSize;
    char *rBuf;
	MPI_File fh;
    MPI_Offset startIndex, endIndex, fileSize;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int prev = rank == 0 ? MPI_PROC_NULL : rank - 1;
    int next = rank == size - 1  ? MPI_PROC_NULL : rank + 1;
    if (rank == 0)
        createFile(FILE, DATA, DATA_REPLICATION_DEGREE);

    MPI_Barrier(MPI_COMM_WORLD);
	MPI_File_open(MPI_COMM_WORLD, FILE, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

    MPI_File_get_size(fh, &fileSize);
    rBufSize = --fileSize / size;
    startIndex = rank * rBufSize;
    endIndex = rank == size - 1 ? fileSize - 1 : startIndex + rBufSize - 1;

    rBufSize = endIndex - startIndex + 1;
    rBuf = new char[rBufSize + 1];

    MPI_File_read_at_all(fh, startIndex, rBuf, rBufSize, MPI_CHAR, MPI_STATUS_IGNORE);
    rBuf[rBufSize] = '\0';

    MPI_Recv(buf, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);

    if (rank == 0)
        *buf = 2021;
    printf("%s", rBuf);

    MPI_Send(buf, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    if (rank == size-1)
        printf("\n");


	MPI_File_close(&fh);
	MPI_Finalize();
    return 0;
}