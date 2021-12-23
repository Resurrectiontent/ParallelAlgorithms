#include <iostream>
#include <mpi.h>
#define FILL_VALUE 1
#define SMOOTHING 10
#define MIN_MESSAGE 1
#define MAX_MESSAGE 1000000
using namespace std;

double avg(double *arr, int length){
    double sum = 0;
    for (int i = 0; i < length; ++i)
        sum += arr[i];
    return sum / length;
}

int main(int argc, char* argv[]) {
    int rank, size;

    int *buf = new int[MAX_MESSAGE];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
        double startTime, finishTime;
        double *localTimesStorage = new double[SMOOTHING];
        int bestBytes = 0;
        float bestBW = .0;
        double bestLat = .0;

        for(int i = MIN_MESSAGE; i <= MAX_MESSAGE; i += 10){
            for (int j = 0; j < SMOOTHING; ++j){
                startTime = MPI_Wtime();
                MPI_Send(buf, i, MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(buf, i, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
                finishTime = MPI_Wtime();

                localTimesStorage[j] = finishTime - startTime;
            }
            int bytes = i * sizeof(int);
            double avgTime = avg(localTimesStorage, SMOOTHING);
            double bandwidth = 2 * bytes /  avgTime / 1024 / 1024;

            if (bandwidth > bestBW){
                bestBW = (float)bandwidth;
                bestBytes = bytes;
                bestLat = avgTime / 2;
            }
            if (i % 100000 == 1)
                cout << "Best for i=" << i << ":\tlen: " << bestBytes << " B\t bandwidth: " << bestBW << " MB/s\t latency: " << bestLat << endl;
        }

        cout << "len: " << bestBytes << " B\t bandwidth: " << bestBW << " MB/s\t latency: " << bestLat << endl;

        delete[](localTimesStorage);
    }
    else if(rank == 1)
        for(int i = MIN_MESSAGE; i <= MAX_MESSAGE; i += 10)
            for (int j = 0; j < SMOOTHING; ++j){
                MPI_Recv(buf, i, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Send(buf, i, MPI_INT, 0, 1, MPI_COMM_WORLD);
            }



    delete[](buf);
    MPI_Finalize();
    return 0;
}