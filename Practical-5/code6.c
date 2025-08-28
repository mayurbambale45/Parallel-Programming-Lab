// Q6: Parallel Sections Example in C
#include <stdio.h>
#include <omp.h>

int main() {
    printf("--- Parallel Sections Example ---\n");

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            printf("Section 1 executed by Thread: %d\n", omp_get_thread_num());
        }

        #pragma omp section
        {
            printf("Section 2 executed by Thread: %d\n", omp_get_thread_num());
        }

        #pragma omp section
        {
            printf("Section 3 executed by Thread: %d\n", omp_get_thread_num());
        }
    }

    return 0;
}