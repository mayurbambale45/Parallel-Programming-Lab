#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <omp.h> 

using namespace std;


void simpleMerge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i]; i++;
        } else {
            arr[k] = R[j]; j++;
        }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

void mergeSort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    simpleMerge(arr, l, m, r);
}

vector<int> merge_two_vectors(const vector<int>& vecA, const vector<int>& vecB) {
    vector<int> merged_data;
    merged_data.reserve(vecA.size() + vecB.size());
    int i = 0, j = 0;
    while (i < vecA.size() && j < vecB.size()) {
        if (vecA[i] <= vecB[j]) merged_data.push_back(vecA[i++]);
        else merged_data.push_back(vecB[j++]);
    }
    while (i < vecA.size()) merged_data.push_back(vecA[i++]);
    while (j < vecB.size()) merged_data.push_back(vecB[j++]);
    return merged_data;
}

int main(int argc, char** argv) {

    double serial_start_time, serial_end_time, serial_time = 0.0;
    double parallel_start_time, parallel_end_time, parallel_time = 0.0;

    int total_N = 10000000;
    vector<int> global_data;
    int num_threads = 1;

    printf("Making %d random numbers to sort...\n", total_N);
    global_data.resize(total_N);
    srand(time(NULL));
    for (int i = 0; i < total_N; ++i) {
        global_data[i] = rand() % 5000;
    }
    printf("Finished making numbers.\n");
    
    vector<int> serial_copy = global_data;
    printf("------------------------------------------------------\n");
    printf("STARTING SERIAL SORT on all %d numbers...\n", total_N);
    serial_start_time = omp_get_wtime();
    mergeSort(serial_copy, 0, total_N - 1);
    serial_end_time = omp_get_wtime();
    serial_time = serial_end_time - serial_start_time;
    printf("Serial Time: %f seconds.\n", serial_time);
    printf("------------------------------------------------------\n\n");
    
    printf("STARTING PARALLEL SORT now.\n\n");


    vector<int> parallel_copy = global_data;
    vector<vector<int>> thread_chunks; 
    parallel_start_time = omp_get_wtime();
    
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        
        if (tid == 0) {
            num_threads = nthreads;
            thread_chunks.resize(nthreads);
        }

        int chunk_size = total_N / nthreads;
        int remainder = total_N % nthreads;
        int my_size = chunk_size + (tid < remainder ? 1 : 0);
        int my_start = (tid * chunk_size) + min(tid, remainder);

        vector<int> local_data;
        if (my_size > 0) {
             local_data.assign(parallel_copy.begin() + my_start, 
                               parallel_copy.begin() + my_start + my_size);
            mergeSort(local_data, 0, my_size - 1);
        }
        thread_chunks[tid] = move(local_data);
    } 
    
    vector<int> final_sorted_data = move(thread_chunks[0]);
    for (int i = 1; i < num_threads; ++i) {
        final_sorted_data = merge_two_vectors(final_sorted_data, thread_chunks[i]);
    }
    
    parallel_end_time = omp_get_wtime();
    parallel_time = parallel_end_time - parallel_start_time;

    printf("------------------------------------------------------\n");
    printf("PARALLEL SORT finished with %d threads.\n", num_threads);
    printf("Parallel Time: %f seconds.\n", parallel_time);
    printf("------------------------------------------------------\n\n");

    printf("======================================================\n");
    printf("FINAL TIME RESULTS (N = %d)\n", total_N);
    printf("======================================================\n");
    printf("Serial Time (1 thread):      %f seconds.\n", serial_time);
    printf("Parallel Time (%d threads):  %f seconds.\n", num_threads, parallel_time);
    
    if (parallel_time < serial_time) {
        printf("\nParallel was %f seconds FASTER!\n", serial_time - parallel_time);
    } else {
        printf("\nParallel was %f seconds SLOWER!\n", parallel_time - serial_time);
    }
    printf("======================================================\n");

    return 0;
}

// /mnt/d/College_Work/SEM-V/PP_Lab/Parallel-Programming-Lab/Practical-8
// g++ 1-project_openmp.cpp -o a -fopenmp -std=c++11 && OMP_NUM_THREADS=10 ./a