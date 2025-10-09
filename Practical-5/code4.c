// Q4: Ordered Loop Execution
#include <stdio.h>
#include <omp.h>

int main() {
    printf("--- Parallel for loop without ordered ---\n");
    #pragma omp parallel for
    for (int i = 1; i <= 20; i++) {
        printf("%d ", i);
    }

    printf("\n\n--- Parallel for loop with #pragma omp ordered ---\n");
    #pragma omp parallel for ordered
    for (int i = 1; i <= 20; i++) {
        #pragma omp ordered
        printf("%d ", i);
    }
    printf("\n");
}
