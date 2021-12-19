#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <chrono>

int** randomMatrix(int size, bool smallInts = false)
{
    int** a = new int*[size];
    for (int i = 0; i < size; ++i)
    {
        a[i] = new int[size];
        for (int j = 0; j < size; ++j)
        {
            a[i][j] = smallInts ? rand() % 10 : rand();
        }
    }

    return a;
}

int** zeroMatrix(int size)
{
    int** a = new int*[size];
    for (int i = 0; i < size; ++i)
    {
        a[i] = new int[size];
        for (int j = 0; j < size; ++j)
        {
            a[i][j] = 0;
        }
    }

    return a;
}

void freeMatrix(int** m, int size)
{
    for (int i = 0; i < size; ++i)
    {
        delete(m[i]);
    }

    delete(m);
}

void printMatrix(int** m, int size)
{
    for (int j = 0; j < size; ++j){
        for(int i = 0; i < size; ++i){
            std::cout<<m[i][j]<<"\t";
        }
        std::cout<<std::endl;
    }
}

/*
i-j-k
i-k-j
j-i-k
j-k-i
k-i-j
k-j-i
*/
int** dotIjk(int** m1, int** m2, int size, int numOfThreads)
{
    int** result = zeroMatrix(size);
#pragma omp parallel for num_threads(numOfThreads)
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                result[j][i] += m1[k][i] * m2[j][k];
            }
        }
    }
    return result;
}
int** dotIkj(int** m1, int** m2, int size, int numOfThreads)
{
    int** result = randomMatrix(size); //content is replaced later
#pragma omp parallel for num_threads(numOfThreads)
    for (int i = 0; i < size; ++i) {
        for (int k = 0; k < size; ++k) {
            for (int j = 0; j < size; ++j) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result;
}
int** dotJik(int** m1, int** m2, int size, int numOfThreads)
{
    int** result = randomMatrix(size); //content is replaced later
#pragma omp parallel for num_threads(numOfThreads)
    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            for (int k = 0; k < size; ++k) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result;
}
int** dotJki(int** m1, int** m2, int size, int numOfThreads)
{
    int** result = randomMatrix(size); //content is replaced later
#pragma omp parallel for num_threads(numOfThreads)
    for (int j = 0; j < size; ++j) {
        for (int k = 0; k < size; ++k) {
            for (int i = 0; i < size; ++i) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result;
}
int** dotKij(int** m1, int** m2, int size, int numOfThreads)
{
    int** result = randomMatrix(size); //content is replaced later
#pragma omp parallel for num_threads(numOfThreads)
    for (int k = 0; k < size; ++k) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result;
}
int** dotKji(int** m1, int** m2, int size, int numOfThreads) {
    int **result = randomMatrix(size); //content is replaced later
#pragma omp parallel for num_threads(numOfThreads)
    for (int k = 0; k < size; ++k) {
        for (int j = 0; j < size; ++j) {
            for (int i = 0; i < size; ++i) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result;
}

int main(int argc, char* argv[])
{
    int mSide = atoi(argv[1]);

    int** m1 = randomMatrix(mSide, mSide == 5);
    int** m2 = randomMatrix(mSide, mSide == 5);
    int** p;
    std::cout<<"Multiply random matrices "<<mSide<<"*"<<mSide<<std::endl;

    if (mSide == 5)
    {
        std::cout<<"M1:"<<std::endl;
        printMatrix(m1, mSide);
        std::cout<<"M2:"<<std::endl;
        printMatrix(m2, mSide);
        p = dotIjk(m1, m2, mSide, 1);
        std::cout<<"Product"<<std::endl;
        printMatrix(p, mSide);
    }
    else
    {
        std::chrono::steady_clock::time_point start, end;
        std::cout<<"Testing different loop orders on 5 threads"<<std::endl;

        //i-j-k
        start = std::chrono::steady_clock::now();
        p = dotIjk(m1, m2, mSide, 5);
        end = std::chrono::steady_clock::now();
        std::cout<<"i-j-k: "<<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()<< "µs"<<std::endl;

        //i-k-j
        start = std::chrono::steady_clock::now();
        p = dotIkj(m1, m2, mSide, 5);
        end = std::chrono::steady_clock::now();
        std::cout<<"i-k-j: "<<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()<< "µs"<<std::endl;

        //j-i-k
        start = std::chrono::steady_clock::now();
        p = dotJik(m1, m2, mSide, 5);
        end = std::chrono::steady_clock::now();
        std::cout<<"j-i-k: "<<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()<< "µs"<<std::endl;

        //j-k-i
        start = std::chrono::steady_clock::now();
        p = dotJki(m1, m2, mSide, 5);
        end = std::chrono::steady_clock::now();
        std::cout<<"j-k-i: "<<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()<< "µs"<<std::endl;

        //k-i-j
        start = std::chrono::steady_clock::now();
        p = dotKij(m1, m2, mSide, 5);
        end = std::chrono::steady_clock::now();
        std::cout<<"k-i-j: "<<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()<< "µs"<<std::endl;

        //k-j-i
        start = std::chrono::steady_clock::now();
        p = dotKji(m1, m2, mSide, 5);
        end = std::chrono::steady_clock::now();
        std::cout<<"k-i-j: "<<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()<< "µs"<<std::endl;

        std::cout<<std::endl<<"Testing different numbers of threads on classic i-j-k loop order"<<std::endl;

        for (int i = 1; i <= 10; ++i)
        {
            start = std::chrono::steady_clock::now();
            p = dotIjk(m1, m2, mSide, i);
            end = std::chrono::steady_clock::now();
            std::cout<<i<<" threads: "<<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()<< "µs"<<std::endl;
        }
    }

    freeMatrix(p, mSide);
    freeMatrix(m1, mSide);
    freeMatrix(m2, mSide);
}