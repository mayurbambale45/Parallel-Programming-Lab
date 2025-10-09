#include <stdio.h>
#include <omp.h>

// Use #define to create a true compile-time constant for array dimensions
#define N 3

int main() {
    // Now the compiler sees these as int A[3][3] and int B[3][3], which is valid.
    int A[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int B[N][N] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    int C[N][N];

    printf("--- Parallel Matrix Addition ---\n");

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] + B[i][j];
            printf("Thread %d computed C[%d][%d] = %d\n", 
                   omp_get_thread_num(), i, j, C[i][j]);
        }
    }

    printf("\nResult Matrix:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}