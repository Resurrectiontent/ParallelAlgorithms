#include <iostream>
#include <mpi.h>
#include <math.h>
#define FILL_VALUE 1
#define ELEMENTS 1000000
using namespace std;

int calculateReceiving(int *map, int rank, int size){
    int* end = map + size;
    int result = -1;
    for (int *i = map; i != end; ++i)
        if (*i == rank)
            ++result;
    return result;
}

int *ones(int n, int v) {
    int *result = new int[n];
    int *end = result + n;
    for(int *i = result; i != end; ++i)
        *i = v;
    return result;
}

void clearMap(int *map, int length){
    int *end = map+length;
    for(int *i = map; i != end; ++i)
        if (*i != i-map)
            *i = -1;
}

void shiftMap(int *map, int length, int power){
    for (int i = 0; i < length; ++i)
        map[i] = map[i] != -1 || power == 0
                ? i - i % (int)pow(2, power)
                : -1;
}

void addVectors(int* dest, int* toAdd, int length){
    for (int i = 0; i < length; ++i)
        dest[i] += toAdd[i];
}

void cascade(int *vector, int rank, int size){
    double startTime = MPI_Wtime();
    int limit = (int)ceil(log2(size));
    int *map = new int[size];
    shiftMap(map, size, 0);

    for (int i = 1; i <= limit; ++i){
        shiftMap(map, size, i);
        int target = map[rank];

        if (rank == target){
            int *buf = new int[ELEMENTS];
            MPI_Status status;

            int receiving = calculateReceiving(map, rank, size);
            for (int j = 0; j < receiving; ++j){
                MPI_Recv(buf, ELEMENTS, MPI_INT,
                         MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
                addVectors(vector, buf, ELEMENTS);
            }

            clearMap(map, size);
            delete[](buf);
        }
        else{
            MPI_Send(vector, ELEMENTS, MPI_INT, target, i, MPI_COMM_WORLD);
            delete[](map);
            return;
        }
    }

    cout << "[CASCADE(exec in " << MPI_Wtime() - startTime << " sec)]:" << endl;
    cout << "Vector of size " << ELEMENTS << " = [" << vector[0] << ", " << vector[1]
         << ", " << vector[2] << ", " << vector[3] << ", " << vector[4] << ", ...];" << endl ;
    delete[](map);
}

void reduce(int *vector, int rank, int size){
    double startTime = MPI_Wtime();
    int *buf = new int[ELEMENTS];

    MPI_Reduce(vector, buf, ELEMENTS,
               MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        cout << "[MPI_Reduce(exec in " << MPI_Wtime() - startTime << " sec)]:" << endl
             << "Vector of size " << ELEMENTS << " = [" << buf[0] << ", " << buf[1]
             << ", " << buf[2] << ", " << buf[3] << ", " << buf[4] << ", ...];" << endl ;
}

int main(int argc, char *argv[]){
    int rank, size;

    int *buf;
    buf = ones(ELEMENTS, FILL_VALUE);
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    cascade(buf, rank, size);
    buf = ones(ELEMENTS, FILL_VALUE);
    MPI_Barrier(MPI_COMM_WORLD);
    reduce(buf, rank, size);

    delete[](buf);
    MPI_Finalize();
    return 0;
}
