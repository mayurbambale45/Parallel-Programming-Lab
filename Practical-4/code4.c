// Q4: Partial sum of 20 numbers using lastprivate
#include <stdio.h>
#include <omp.h>

#define N 20

int main() {
    int final_sum = 0;

    printf("--- Lastprivate Clause Example ---\n");
    #pragma omp parallel for lastprivate(final_sum)
    for (int i = 1; i <= N; i++) {
        final_sum += i;
    }

    printf("Final sum: %d\n", final_sum);
    return 0;
}
