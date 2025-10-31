// Problem Statement 2: Vector Dot Product
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h> // <-- Add this for sin/cos

#define N 10000000

int main() {
    double *A, *B;
    double result_serial = 0.0, result_parallel = 0.0;
    int i;

    A = (double*)malloc(N * sizeof(double));
    B = (double*)malloc(N * sizeof(double));

    // Initialize with different values for sin/cos to prevent
    // the compiler from optimizing everything away
    for (i = 0; i < N; i++) {
        A[i] = i * 0.0001;
        B[i] = i * 0.0002;
    }

    printf("--- Serial Heavy Computation ---\n");
    double start_serial = omp_get_wtime();
    for (i = 0; i < N; i++) {
        // Make the work "harder" (compute-bound)
        result_serial += sin(A[i]) * cos(B[i]);
    }
    double end_serial = omp_get_wtime();
    printf("Result: %e\n", result_serial);
    printf("Time taken: %f seconds\n", end_serial - start_serial);

    printf("\n--- Parallel Heavy Computation ---\n");
    double start_parallel = omp_get_wtime();
    
    // The pragma is the same and is correct
    #pragma omp parallel for reduction(+:result_parallel)
    for (i = 0; i < N; i++) {
        // Make the work "harder" (compute-bound)
        result_parallel += sin(A[i]) * cos(B[i]);
    }
    double end_parallel = omp_get_wtime();
    printf("Result: %e\n", result_parallel);
    printf("Time taken: %f seconds\n", end_parallel - start_parallel);

    free(A);
    free(B);

    return 0;
}

// gcc -fopenmp code2.c -o a && ./a
// gcc -O3 -fopenmp code2.c -o a -lm && ./a
