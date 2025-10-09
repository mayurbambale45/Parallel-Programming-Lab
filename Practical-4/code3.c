// Q3: Private variable example
#include <stdio.h>
#include <omp.h>

int main() {
    int Aryabhatta = 10;

    printf("--- Multiplication with private 'Aryabhatta' ---\n");
    #pragma omp parallel firstprivate(Aryabhatta)
    {
        int id = omp_get_thread_num();
        int result = id * Aryabhatta;
        printf("Thread ID: %d, Result: %d\n", id, result);
    }
    return 0;
}
