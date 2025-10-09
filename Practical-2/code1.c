#include <stdio.h>
#include <omp.h>
#define N 5

int main() {
    int a[N] = {1, 2, 3, 4, 5};
    int b[N] = {6, 7, 8, 9, 10};
    int c[N];

    omp_set_num_threads(4);

    printf("Correct method with #pragma omp parallel for:- \n");
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
        printf("Thread %d processed index %d, result %d\n", omp_get_thread_num(), i, c[i]);
    }

    printf("\nIncorrect method with #pragma omp parallel:- \n");
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        for (int i = 0; i < N; i++) {
            c[i] = a[i] + b[i];
            printf("Thread %d processed index %d, result %d\n", tid, i, c[i]);
        }
    }
    return 0;
}