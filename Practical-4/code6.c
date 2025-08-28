// Q6: Parallel Series Print using Sections
#include <stdio.h>
#include <omp.h>

int main() {
    printf("--- Parallel Series Print ---\n");
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            printf("Series of 2 by thread %d: ", omp_get_thread_num());
            for (int i = 1; i <= 10; i++) {
                printf("%d ", 2 * i);
            }
            printf("\n");
        }
        #pragma omp section
        {
            printf("Series of 4 by thread %d: ", omp_get_thread_num());
            for (int i = 1; i <= 10; i++) {
                printf("%d ", 4 * i);
            }
            printf("\n");
        }
    }
    return 0;
}