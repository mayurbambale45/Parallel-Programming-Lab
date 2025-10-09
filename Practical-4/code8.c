// Q8: Parallel sum with reduction
#include <stdio.h>
#include <omp.h>

#define N 10000000

int main() {
    long long sum_serial = 0, sum_parallel = 0;

    printf("--- Serial Summation ---\n");
    double start_serial = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        sum_serial += 1;
    }
    double end_serial = omp_get_wtime();
    printf("Sum: %lld\nTime: %f seconds\n", sum_serial, end_serial - start_serial);

    printf("\n--- Parallel Summation with Reduction ---\n");
    double start_parallel = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum_parallel)
    for (int i = 0; i < N; i++) {
        sum_parallel += 1;
    }
    double end_parallel = omp_get_wtime();
    printf("Sum: %lld\nTime: %f seconds\n", sum_parallel, end_parallel - start_parallel);

    return 0;
}
