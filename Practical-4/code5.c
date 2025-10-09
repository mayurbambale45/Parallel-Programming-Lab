// Q5: Matrix multiplication with static and dynamic scheduling
#include <stdio.h>
#include <omp.h>

#define N 512
int A[N][N], B[N][N], C[N][N];

void initialize_matrices() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0;
        }
    }
}

int main() {
    initialize_matrices();

    printf("--- Serial Matrix Multiplication ---\n");
    double start_serial = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    double end_serial = omp_get_wtime();
    printf("Time taken: %f seconds\n", end_serial - start_serial);

    initialize_matrices();
    printf("\n--- Static Scheduling ---\n");
    double start_static = omp_get_wtime();
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    double end_static = omp_get_wtime();
    printf("Time taken: %f seconds\n", end_static - start_static);

    initialize_matrices();
    printf("\n--- Dynamic Scheduling ---\n");
    double start_dynamic = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    double end_dynamic = omp_get_wtime();
    printf("Time taken: %f seconds\n", end_dynamic - start_dynamic);

    return 0;
}
