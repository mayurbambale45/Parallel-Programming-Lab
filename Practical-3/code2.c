#include <stdio.h>
#include <omp.h>
#define N 1000000

int main() {
    static int vec[N];
    int scalar = 10;

    double start = omp_get_wtime();
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        vec[i] += scalar;
    }
    double end = omp_get_wtime();
    printf("Static Time: %f seconds\n", end - start);

    return 0;
}