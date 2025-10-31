// Problem Statement 1: Matrix Matrix Multiplication
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1024

int main() {
    int **A, **B, **C;
    int i, j, k;

    // --- 1. Memory Allocation ---
    A = (int **)malloc(N * sizeof(int *));
    B = (int **)malloc(N * sizeof(int *));
    C = (int **)malloc(N * sizeof(int *));

    int *A_data = (int *)malloc(N * N * sizeof(int));
    int *B_data = (int *)malloc(N * N * sizeof(int));
    int *C_data = (int *)malloc(N * N * sizeof(int));

    for (i = 0; i < N; i++) {
        A[i] = &(A_data[i * N]);
        B[i] = &(B_data[i * N]);
        C[i] = &(C_data[i * N]);
    }

    // Initialize matrices A and B
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0;
        }
    }

    printf("--- Serial Matrix Multiplication (N=%d) ---\n", N);
    double start_serial = omp_get_wtime();
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    double end_serial = omp_get_wtime();
    printf("Time taken: %f seconds\n", end_serial - start_serial);


    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            C[i][j] = 0;

    printf("\n--- Parallel Matrix Multiplication (N=%d) ---\n", N);
    double start_parallel = omp_get_wtime();
    
    // The pragma is correct!
    #pragma omp parallel for collapse(2) private(i,j,k)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    double end_parallel = omp_get_wtime();
    printf("Time taken: %f seconds\n", end_parallel - start_parallel);

    return 0;
}


// gcc -fopenmp code1.c -o a && ./a
// gcc -03 -fopenmp code1.c -o a && ./a