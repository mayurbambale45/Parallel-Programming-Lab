// Q9: Calculation of Pi using reduction
#include <stdio.h>
#include <omp.h>

#define N 100000000

int main() {
    double sum = 0.0, pi = 0.0, step = 1.0 / N;

    printf("--- Pi Calculation using OpenMP Reduction ---\n");
    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (long long i = 0; i < N; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    pi = sum * step;

    double end_time = omp_get_wtime();
    printf("Pi value: %.15f\n", pi);
    printf("Time taken: %f seconds\n", end_time - start_time);

    return 0;
}
