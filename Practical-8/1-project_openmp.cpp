#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <omp.h>    // OpenMP Header

using namespace std;

// --- Sequential Merge and MergeSort (UNCHANGED) ---
// These are used by each thread to sort its *local* chunk.
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

// --- NEW HELPER: Merges two *already sorted* vectors ---
vector<int> merge_two_vectors(const vector<int>& vecA, const vector<int>& vecB) {
    vector<int> merged_data;
    merged_data.reserve(vecA.size() + vecB.size()); // Good practice
    int i = 0, j = 0;
    while (i < vecA.size() && j < vecB.size()) {
        if (vecA[i] <= vecB[j]) merged_data.push_back(vecA[i++]);
        else merged_data.push_back(vecB[j++]);
    }
    while (i < vecA.size()) merged_data.push_back(vecA[i++]);
    while (j < vecB.size()) merged_data.push_back(vecB[j++]);
    return merged_data;
}


// ===============================================
// ===        THE MAIN PROGRAM!              ===
// ===============================================
int main(int argc, char** argv) {

    double serial_start_time, serial_end_time, serial_time = 0.0;
    double parallel_start_time, parallel_end_time, parallel_time = 0.0;

    int total_N = 10000000;
    vector<int> global_data;
    int num_threads = 1;

    // --- 1. SETUP & DATA GENERATION ---
    printf("Generating %d random numbers to sort...\n", total_N);
    global_data.resize(total_N);
    srand(time(NULL));
    for (int i = 0; i < total_N; ++i) {
        global_data[i] = rand() % 5000;
    }
    printf("...Done generating numbers.\n");
    
    // --- SERIAL (SEQUENTIAL) SORT ---
    vector<int> serial_copy = global_data;
    printf("------------------------------------------------------\n");
    printf("SERIAL SORT: I am sorting all %d numbers by myself...\n", total_N);
    serial_start_time = omp_get_wtime();
    mergeSort(serial_copy, 0, total_N - 1);
    serial_end_time = omp_get_wtime();
    serial_time = serial_end_time - serial_start_time;
    printf(">>> Serial (Sequential) Time: %f seconds <<<\n", serial_time);
    printf("------------------------------------------------------\n\n");
    
    printf("OK, now let's try it with the whole team (Parallel)!\n\n");

    // --- 2. PARALLEL SORT (THE *NEW* FAST WAY) ---
    vector<int> parallel_copy = global_data;
    
    // We need a place to store the sorted chunks from each thread
    // We get num_threads from the environment (e.g., OMP_NUM_THREADS=8)
    // This `thread_chunks` vector will hold 8 sorted vectors.
    vector<vector<int>> thread_chunks; 

    parallel_start_time = omp_get_wtime();

    // === STEP 1: Parallel Region & Local Sort (The "Scatter") ===
    #pragma omp parallel
    {
        // Find out who I am and how many of us there are
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        
        // This is the FIRST TIME we set num_threads, do it only once
        if (tid == 0) {
            num_threads = nthreads;
            // Resize the outer vector to hold the result from each thread
            thread_chunks.resize(nthreads);
        }

        // Calculate my personal chunk, just like in MPI
        int chunk_size = total_N / nthreads;
        int remainder = total_N % nthreads;
        int my_size = chunk_size + (tid < remainder ? 1 : 0);
        int my_start = (tid * chunk_size) + min(tid, remainder);

        // === THIS IS THE KEY! ===
        // Each thread creates its *OWN PRIVATE* vector
        // It copies its chunk from the main 'parallel_copy'
        vector<int> local_data;
        if (my_size > 0) {
             local_data.assign(parallel_copy.begin() + my_start, 
                               parallel_copy.begin() + my_start + my_size);
            
            // Now, sort *ONLY MY PRIVATE* vector.
            // NO CACHE CONTENTION!
            mergeSort(local_data, 0, my_size - 1);
        }

        // Save my sorted chunk into the shared `thread_chunks` vector
        // This is safe because each thread writes to its *own slot* (tid)
        thread_chunks[tid] = move(local_data);

        // No barrier needed, the parallel region ends and syncs
    } // --- All threads sync here ---

    // === STEP 2: The Final Merge (The "Gather") ===
    // Now we have N/8 sorted chunks. We just merge them.
    // We can just let one thread (the main thread) do this.
    // It's fast because the hard sorting work is done.
    
    // Take the first chunk as the base
    vector<int> final_sorted_data = move(thread_chunks[0]);
    
    // Merge all the other chunks (1 through num_threads-1)
    for (int i = 1; i < num_threads; ++i) {
        final_sorted_data = merge_two_vectors(final_sorted_data, thread_chunks[i]);
    }
    
    // STOP THE PARALLEL CLOCK!
    parallel_end_time = omp_get_wtime();
    parallel_time = parallel_end_time - parallel_start_time;

    printf("------------------------------------------------------\n");
    printf("PARALLEL SORT: All %d threads finished the local sorts & merge!\n", num_threads);
    printf(">>> Parallel Time: %f seconds <<<\n", parallel_time);
    printf("------------------------------------------------------\n\n");

    
    // --- 6. FINAL RESULT & COMPARISON ---
    printf("======================================================\n");
    printf("           FINAL TIME COMPARISON (N = %d)\n", total_N);
    printf("======================================================\n");
    printf("Serial Time (1 thread):      %f seconds\n", serial_time);
    printf("Parallel Time (%d threads):   %f seconds\n", num_threads, parallel_time);
    
    if (parallel_time < serial_time) {
        printf("\n>>> HOORAY! Parallel was %f seconds FASTER! <<<\n", serial_time - parallel_time);
    } else {
        printf("\n>>> HUH! Parallel was %f seconds SLOWER! <<<\n", parallel_time - serial_time);
    }
    printf("======================================================\n");

    return 0;
}

// /mnt/d/College_Work/SEM-V/PP_Lab/Parallel-Programming-Lab/Practical-8
// g++ 1-project_openmp.cpp -o b -fopenmp -std=c++11 && export OMP_NUM_THREADS=10 && ./b