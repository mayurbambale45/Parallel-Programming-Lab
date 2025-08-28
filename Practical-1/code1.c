#include <stdio.h>
#include <omp.h>

int main() {
    int n;

    printf("Number of threads: ");
    scanf("%d", &n);

    printf("\n--- Sequential Output ---\n");
    for (int i = 0; i < n; i++) {
        printf("Hello, World from thread = %d (sequential)\n", i);
    }

    printf("\n--- Parallel Output ---\n");
    
    omp_set_num_threads(n);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Hello, World from thread = %d (parallel)\n", tid);
    }

    return 0;
}