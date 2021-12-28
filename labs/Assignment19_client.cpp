#include "cstdio"
#include "string"
#include "mpi.h"
#define SENDING_VALUE 42
using namespace std;

int main(int argc, char **argv)
{
	int *rcv = new int[1];
    int snd[1] = {SENDING_VALUE};
	MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm intercomm;
    MPI_Request sending, receiving;
    string len(argv[1]);
    char port_name[len.length()];
	strcpy(port_name, argv[1]);

    printf("Attempt to connect %s\n", port_name);
	MPI_Comm_connect(argv[1], MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    printf("Connected to the server\n");

    printf("Client connected\n");
    MPI_Isend(&snd,1,MPI_INT,0,0,intercomm, &sending);
    printf("Client sent value: %d\n", snd[0]);

    MPI_Irecv(rcv, 1, MPI_INT, 0, 0, intercomm, &receiving);
    MPI_Wait(&receiving, &status);
    printf("Client received value: %d\n", *rcv);

	MPI_Finalize();
	return 0;
}