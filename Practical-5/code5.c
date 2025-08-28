// Q5: Family Members with Thread IDs
#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
using namespace std;

int main() {
    vector<string> family = {"Father", "Mother", "Brother", "Sister"};

    cout << "--- Family Members and Thread IDs ---" << endl;

    #pragma omp parallel for
    for (int i = 0; i < family.size(); i++) {
        cout << "Thread ID: " << omp_get_thread_num()
             << ", Name: " << family[i] << endl;
    }

    return 0;
}
