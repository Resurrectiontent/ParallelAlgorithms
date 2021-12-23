#include <iostream>
#include <mpi.h>
#define FILL_VALUE 1
using namespace std;

long* generateVector(int length){
    long* result = new long [length];
    for(int i = 0; i < length; ++i){
        result[i] = (long)FILL_VALUE;
    }
    return result;
}

long calculateSum(long* v1, long* v2, int length){
    long sum = 0;
    for(int i = 0; i <length; ++i){
        sum += v1[i] * v2[i];
    }
    return sum;
}

int main(int argc, char* argv[]) {
    int rank, size;
    long result;
    MPI_Status status;
    MPI_Init(&argc, &argv);

    int length = atoi(argv[1]);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    long *rbuf = new long[size];
    int normalPortion = length / (size-1);

    if (rank == 0){
        long* vector1 = generateVector(length);
        long* vector2 = generateVector(length);

        long checkResult = calculateSum(vector1, vector2, length);

        for (int i = 1; i < size; ++i){
            int portion = i == size-1 ? length - normalPortion * (size-2) : normalPortion;
            int offset = normalPortion * (i-1);
            MPI_Send(vector1+offset, portion, MPI_LONG, i, 0, MPI_COMM_WORLD);
            MPI_Send(vector2+offset, portion, MPI_LONG, i, 1, MPI_COMM_WORLD);
        }

        long toSend = 0l;
        MPI_Gather(&toSend, 1, MPI_LONG, rbuf, 1, MPI_LONG, 0, MPI_COMM_WORLD);

        delete[](vector1);
        delete[](vector2);

        long sum = 0;
        for (int i = 1; i < size; ++i) {
            sum += rbuf[i];
        }

        cout << "Total: " << sum << "; Check: " << checkResult << " => " << (sum == checkResult ? "Success" : "Fail") << endl;
    }
    else{
        int portion = rank == size-1 ? length - normalPortion * (size-2) : normalPortion;

        long* receiver1 = new long[portion];
        long* receiver2 = new long[portion];

        MPI_Recv(receiver1, portion, MPI_LONG, 0,
                 0, MPI_COMM_WORLD, &status);
        MPI_Recv(receiver2, portion, MPI_LONG, 0,
                 1, MPI_COMM_WORLD, &status);

        result = calculateSum(receiver1, receiver2, portion);

        MPI_Gather(&result, 1, MPI_LONG, rbuf,
                   1, MPI_LONG, 0, MPI_COMM_WORLD);

        delete[](receiver1);
        delete[](receiver2);
    }
    delete[](rbuf);
    MPI_Finalize();
    return 0;
}