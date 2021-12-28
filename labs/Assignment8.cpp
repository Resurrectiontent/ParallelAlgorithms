#include <iostream>
#include <mpi.h>
#define FILL_VALUE 1
#define SMOOTHING 10
#define MIN_MESSAGE 1
#define MAX_MESSAGE     1000000
#define LOGGING_PERIOD  100000
using namespace std;

double avg(double *arr, int length){
    double sum = 0;
    for (int i = 0; i < length; ++i)
        sum += arr[i];
    return sum / length;
}
float avg(float *arr, int length){
    float sum = 0;
    for (int i = 0; i < length; ++i)
        sum += arr[i];
    return sum / (float)length;
}
int avg(int *arr, int length){
    int sum = 0;
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
        int *allBytes = new int[LOGGING_PERIOD];
        double *allBWs = new double[LOGGING_PERIOD];
        double *allLats = new double[LOGGING_PERIOD];
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

            allBytes[i % LOGGING_PERIOD] = bytes;
            allBWs[i % LOGGING_PERIOD] = bandwidth;

            if (bandwidth > bestBW){
                bestBW = (float)bandwidth;
                bestBytes = bytes;
                bestLat = avgTime / 2;
            }
            if (i == 1){
                cout << "For the first measurement:\t\t" << (float)bytes / 1024 / 1024 << " MB \t" << bandwidth << " MB/s" << endl;
            }
            else if (i % LOGGING_PERIOD == 1)
                cout << "Avg values for measurements " << i-LOGGING_PERIOD << "-" << i << ":\t"
                    << (float)bytes / 1024 / 1024 << " MB \t" << bandwidth << " MB/s" << endl;
        }

        cout << "The Best results:\t len: " << bestBytes << " B\t bandwidth: " << bestBW << " MB/s\t latency: " << bestLat << endl;

        delete[](localTimesStorage);
        delete[](allBWs);
        delete[](allLats);
        delete[](allBytes);
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