#include "cstdio"
#include "mpi.h"
#include "string"
#include "fstream"
#define BUFSIZE 100
#define FILE "file.txt"

using namespace std;

void createFile(string name){
    ofstream outfile;
    outfile.open(name.c_str());
    outfile << "The romans came to England in 43 B.C." << endl
        << "They built a town or river Themse and called it Londinium";
    outfile.close();
}

int main(int argc, char **argv)
{
    int bufsize, num, sum;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_File fh;
    char buf[BUFSIZE];
    createFile(FILE);
    MPI_File_open(MPI_COMM_WORLD, FILE, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    sum = 0;
    do {
        MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
        MPI_Get_count(&status, MPI_CHAR, &num);
        printf("buf = \"%s\" (%d sym);\n", buf, num);
        sum += num;
    } while (num >= BUFSIZE);
    MPI_File_close(&fh);
    printf("Total symbols read: %d\n", sum);
	MPI_Finalize();
}