// Q7: Parallel Matrix Addition
#include<iostream>
#include<omp.h>
using namespace std;

int main() {
    const int N = 3;
    int A[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int B[N][N] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    int C[N][N];

    cout << "--- Parallel Matrix Addition ---" << endl;

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = A[i][j] + B[i][j];
            cout << "Thread " << omp_get_thread_num()
                 << " computed C[" << i << "][" << j << "] = "
                 << C[i][j] << endl;
        }
    }

    cout << "\nResult Matrix:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
