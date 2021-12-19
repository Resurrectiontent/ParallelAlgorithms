#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <chrono>

void fillArray(int arr[], int size)
{
    for (int i = 0; i < size; ++i)
        arr[i] = rand();

    if (size == 10)
        for (int i = 0; i < size; ++i)
        {
            std::cout<<"El "<<i<<"="<<arr[i]<<std::endl;
        }
}

void testThreads(int arr[], int size, int numOfThreads)
{
    int maxEl = 0;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

#pragma omp parallel for reduction(max:maxEl) firstprivate(arr) num_threads(numOfThreads)
    for (long i = 0; i < size; ++i)
    {
        if (arr[i] > maxEl)
            maxEl = arr[i];
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout <<numOfThreads<< " threads exec in " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "µs"
              << std::endl;
}

int main(int argc, char* argv[])
{
    const int NOT = 3;

    long size = atol(argv[1]);

    int *array = new int[size];
    fillArray(array, size);

    if (size == 10)
        testThreads(array, size, NOT);
    else
        for (int i = 0; i < 10; ++i)
        {
            testThreads(array, size, i + 1);
        }

    delete[] array;
}