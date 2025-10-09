// Q3: Barrier Synchronization
#include <stdio.h>
#include <omp.h>

int main() {
    int num_threads = omp_get_max_threads();
    int squares[num_threads];

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        squares[id] = id * id;

        #pragma omp barrier

        #pragma omp master
        {
            printf("--- Barrier Synchronization ---\n");
            for (int i = 0; i < num_threads; i++) {
                printf("Thread %d squared: %d\n", i, squares[i]);
            }
        }
    }
    return 0;
}
