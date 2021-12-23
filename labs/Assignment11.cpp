#include <iostream>
#include <mpi.h>
using namespace std;

void giveAStick(int rank, int *buf) {
    cout << "S"  << rank << "->" << rank + 1;
    MPI_Send(buf, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD); }

void waitAStick(int rank) {
    int* stick;
    MPI_Status status;
    cout << "R"  << rank << "<-" << rank - 1;
    MPI_Recv(stick, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
    delete(stick);
}

void printState(int rank, int prev, int next){
    cout << prev << " -> P[" << rank << "] <- " << next << endl;
}

int main(int argc, char **argv)
{
	int rank, size, prev, next;
	int *buf = new int[2];
	MPI_Init(&argc, &argv);
	MPI_Request reqs[4];
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	prev = rank == 0 ? size - 1 : rank - 1;
	next = rank == size - 1 ? 0 : rank + 1;

    if (rank == 0){
        MPI_Sendrecv(&rank, 1, MPI_INT, next, 0, buf+1, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &status);
        MPI_Sendrecv(&rank, 1, MPI_INT, prev, 0, buf, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
    }
    else{
        MPI_Sendrecv(&rank, 1, MPI_INT, prev, 0, buf, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
        MPI_Sendrecv(&rank, 1, MPI_INT, next, 0, buf+1, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &status);
    }

	/*MPI_Irecv(buf, 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
	MPI_Irecv(buf+1, 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]);
	MPI_Isend(&rank, 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
	MPI_Isend(&rank, 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]);
	MPI_Waitall(4, reqs, status);*/

	//Your code here.
	//Here you need to display the number of the current process, and what it receives from the previous and next processes.
	/*if (rank == 0){
        printState(rank, buf[0], buf[1]);
        giveAStick(rank, buf);
    }
    else if (rank == size - 1){
        waitAStick(rank);
        printState(rank, buf[0], buf[1]);
    }
    else{
        waitAStick(rank);
        printState(rank, buf[0], buf[1]);
        giveAStick(rank, buf);
    }*/
    printState(rank, buf[0], buf[1]);

	MPI_Finalize();
    return 0;
}
