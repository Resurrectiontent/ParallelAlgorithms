#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv){
    int size, rank, prev, next;
    MPI_Status stats[4];
    MPI_Request reqs[4];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rbuf[2] = {rank+1, rank * 1000+1000};
    int sbuf[2];
    prev = rank == 0 ? size - 1 : rank - 1;
    next = rank == size - 1 ? 0 : rank + 1;

    MPI_Recv_init(rbuf, 1, MPI_FLOAT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
    MPI_Recv_init(rbuf + 1, 1, MPI_FLOAT, next, 6, MPI_COMM_WORLD, &reqs[1]);
    MPI_Send_init(sbuf, 1, MPI_FLOAT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
    MPI_Send_init(sbuf + 1, 1, MPI_FLOAT, next, 5, MPI_COMM_WORLD, &reqs[3]);
    if (rank == 0 ) cout << "P1 init " << rbuf[0] << ", " << rbuf[1] << endl;
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < size; ++i) {
        sbuf[0] = rbuf[1];
        sbuf[1] = rbuf[0];
        MPI_Startall(4, reqs);
        MPI_Waitall(4, reqs, stats);
        if (rank == 0 ) cout << "P1 recvd " << rbuf[0] << ", " << rbuf[1] << endl;
    }
    MPI_Request_free(&reqs[0]);
    MPI_Request_free(&reqs[1]);
    MPI_Request_free(&reqs[2]);
    MPI_Request_free(&reqs[3]);

    MPI_Finalize();
    return 0;
}