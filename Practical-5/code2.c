// Q2: Array Sum (Atomic vs Critical)
#include <stdio.h>
#include <omp.h>

#define N 1000000

int main() {
    long long sum_atomic = 0;
    printf("--- Sum with #pragma omp atomic ---\n");
    double start_atomic = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 1; i <= N; i++) {
        #pragma omp atomic
        sum_atomic += i;
    }
    double end_atomic = omp_get_wtime();
    printf("Final sum: %lld\n", sum_atomic);
    printf("Time taken: %f seconds\n", end_atomic - start_atomic);

    long long sum_critical = 0;
    printf("\n--- Sum with #pragma omp critical ---\n");
    double start_critical = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 1; i <= N; i++) {
        #pragma omp critical
        sum_critical += i;
    }
    double end_critical = omp_get_wtime();
    printf("Final sum: %lld\n", sum_critical);
    printf("Time taken: %f seconds\n", end_critical - start_critical);

    return 0;
}
