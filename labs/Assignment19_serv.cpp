#include <cstdio>
#include "mpi.h"
#define SENDING_VALUE 25

int main(int argc, char **argv)
{
	int *rcv = new int[1];
    int snd[1]= {SENDING_VALUE};
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Status status;
	MPI_Comm intercomm;
    MPI_Request sending, receiving;
	MPI_Open_port(MPI_INFO_NULL, port_name);
	printf("portname: %s\nWaiting for the client...\n", port_name);

	MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);

    printf("Client connected\n");
    MPI_Isend(&snd,1,MPI_INT,0,0,intercomm, &sending);
    printf("Server sent value: %d\n", snd[0]);

	MPI_Irecv(rcv, 1, MPI_INT, 0, 0, intercomm, &receiving);
    MPI_Wait(&receiving, &status);
    printf("Server received value: %d\n", *rcv);

	MPI_Comm_free(&intercomm);
	MPI_Close_port(port_name);
	MPI_Finalize();

	return 0;
}