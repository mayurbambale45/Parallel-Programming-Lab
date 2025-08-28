// Problem Statement 1: Matrix Matrix Multiplication
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 256

int main() {
    int A[N][N], B[N][N], C[N][N];
    int i, j, k;

    // Initialize matrices A and B
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0;
        }
    }

    printf("--- Serial Matrix Multiplication ---\n");
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

    // Reset matrix C
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            C[i][j] = 0;

    printf("\n--- Parallel Matrix Multiplication ---\n");
    double start_parallel = omp_get_wtime();
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
