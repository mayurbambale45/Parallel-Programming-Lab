#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_num_threads(4);

    printf("--- Using #pragma omp parallel ---\n");
    #pragma omp parallel
    {
        for (int i = 0; i < 4; i++) {
            printf("Hello from thread %d, i = %d\n", omp_get_thread_num(), i);
        }
    }

    printf("\n--- Using #pragma omp parallel for ---\n");
    #pragma omp parallel for
    for (int i = 0; i < 4; i++) {
        printf("Hello from thread %d, i = %d\n", omp_get_thread_num(), i);
    }

    return 0;
}