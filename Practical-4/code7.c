// Q7: Synchronization with critical and atomic
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

const int N = 1000000;

int main() {
    int *data = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        data[i] = i + 1;
    }

    long long total_sum_no_sync = 0;
    printf("--- Unsynchronized Sum ---\n");
    double start_no_sync = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        total_sum_no_sync += data[i];
    }
    double end_no_sync = omp_get_wtime();
    printf("Sum: %lld\n", total_sum_no_sync);
    printf("Time: %f seconds\n", end_no_sync - start_no_sync);

    long long total_sum_critical = 0;
    printf("\n--- Sum with critical ---\n");
    double start_critical = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp critical
        total_sum_critical += data[i];
    }
    double end_critical = omp_get_wtime();
    printf("Sum: %lld\n", total_sum_critical);
    printf("Time: %f seconds\n", end_critical - start_critical);

    long long total_sum_atomic = 0;
    printf("\n--- Sum with atomic ---\n");
    double start_atomic = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp atomic
        total_sum_atomic += data[i];
    }
    double end_atomic = omp_get_wtime();
    printf("Sum: %lld\n", total_sum_atomic);
    printf("Time: %f seconds\n", end_atomic - start_atomic);

    free(data);
    return 0;
}