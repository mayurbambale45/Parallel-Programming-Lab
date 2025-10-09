// Q1: Shared Counter Increment (Critical vs No Sync)
#include <stdio.h>
#include <omp.h>

#define N 1000000

int main() {
    int counter_no_sync = 0;
    printf("--- Counter without synchronization ---\n");
    double start_no_sync = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        counter_no_sync++;
    }
    double end_no_sync = omp_get_wtime();
    printf("Final counter value: %d\n", counter_no_sync);
    printf("Time taken: %f seconds\n", end_no_sync - start_no_sync);

    int counter_critical = 0;
    printf("\n--- Counter with #pragma omp critical ---\n");
    double start_critical = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp critical
        counter_critical++;
    }
    double end_critical = omp_get_wtime();
    printf("Final counter value: %d\n", counter_critical);
    printf("Time taken: %f seconds\n", end_critical - start_critical);

    return 0;
}
