// Q5: Family Members with Thread IDs in C
#include <stdio.h>
#include <omp.h>

int main() {
    // In C, an array of strings is represented as an array of character pointers.
    const char *family[] = {"Father", "Mother", "Brother", "Sister"};
    
    // We calculate the number of elements in the array manually.
    int n = sizeof(family) / sizeof(family[0]);

    printf("--- Family Members and Thread IDs ---\n");

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        // Use printf for formatted output in C.
        printf("Thread ID: %d, Name: %s\n", omp_get_thread_num(), family[i]);
    }

    return 0;
}