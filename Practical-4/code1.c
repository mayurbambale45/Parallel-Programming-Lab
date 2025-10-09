// Q1: Print family members with thread IDs
#include <stdio.h>
#include <omp.h>

int main() {
    const char *family[] = {"Father", "Mother", "Brother", "Sister"};
    int n = 4;

    printf("--- Family Members and Thread IDs ---\n");
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        printf("Thread ID: %d, Name: %s\n", omp_get_thread_num(), family[i]);
    }
    return 0;
}
