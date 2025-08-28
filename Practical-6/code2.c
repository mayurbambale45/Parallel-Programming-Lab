// Problem Statement 2: Vector Dot Product
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000

int main() {
    double *A, *B;
    double dot_product_serial = 0.0, dot_product_parallel = 0.0;
    int i;

    A = (double*)malloc(N * sizeof(double));
    B = (double*)malloc(N * sizeof(double));

    for (i = 0; i < N; i++) {
        A[i] = i * 1.0;
        B[i] = i * 2.0;
    }

    printf("--- Serial Vector Dot Product ---\n");
    double start_serial = omp_get_wtime();
    for (i = 0; i < N; i++) {
        dot_product_serial += A[i] * B[i];
    }
    double end_serial = omp_get_wtime();
    printf("Dot Product: %e\n", dot_product_serial);
    printf("Time taken: %f seconds\n", end_serial - start_serial);

    printf("\n--- Parallel Vector Dot Product ---\n");
    double start_parallel = omp_get_wtime();
    #pragma omp parallel for reduction(+:dot_product_parallel)
    for (i = 0; i < N; i++) {
        dot_product_parallel += A[i] * B[i];
    }
    double end_parallel = omp_get_wtime();
    printf("Dot Product: %e\n", dot_product_parallel);
    printf("Time taken: %f seconds\n", end_parallel - start_parallel);

    free(A);
    free(B);

    return 0;
}
