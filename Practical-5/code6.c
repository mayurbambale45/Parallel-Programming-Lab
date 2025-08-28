// Q6: Parallel Sections Example
#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    cout << "--- Parallel Sections Example ---" << endl;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            cout << "Section 1 executed by Thread: "
                 << omp_get_thread_num() << endl;
        }

        #pragma omp section
        {
            cout << "Section 2 executed by Thread: "
                 << omp_get_thread_num() << endl;
        }

        #pragma omp section
        {
            cout << "Section 3 executed by Thread: "
                 << omp_get_thread_num() << endl;
        }
    }

    return 0;
}
