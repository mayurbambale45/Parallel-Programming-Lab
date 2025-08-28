// Q2: Sum of squares of thread IDs
#include <stdio.h>
#include <omp.h>

int main() {
    int sum_squares = 0;

    printf("--- Squares of Thread IDs ---\n");
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int square = id * id;
        printf("Thread ID: %d, Square: %d\n", id, square);

        #pragma omp atomic
        sum_squares += square;
    }

    printf("\n--- Final Sum of Squares ---\n");
    printf("Total Sum: %d\n", sum_squares);
    return 0;
}
