#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()
#include <stdio.h>    // For printf
#include <mpi.h>

using namespace std;

// --- A simple merge function for our local sequential sort ---
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

// --- Our simple, recursive Merge Sort ---
void mergeSort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    simpleMerge(arr, l, m, r);
}

// --- Helper to print our array (we won't use it, but good to keep) ---
void printArray(const char* title, const vector<int>& arr) {
    printf("%s", title);
    for (int i = 0; i < arr.size(); ++i) {
        if (i > 0 && i % 20 == 0) printf("\n       ");
        printf("%d ", arr[i]);
    }
    printf("\n\n");
}


// ===============================================
// ===          THE MAIN PROGRAM!              ===
// ===============================================
int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // My process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size); // How many processes total

    // Timer variables
    double serial_start_time, serial_end_time, serial_time = 0.0;
    double parallel_start_time, parallel_end_time, parallel_time = 0.0;

    int total_N = 10000000; // --- HERE IS THE CHANGE! ---
    vector<int> global_data;

    // --- 1. SETUP & SERIAL SORT (Only 'Boss' Process 0 does this) ---
    if (rank == 0) {
        printf("Hey! I'm the 'Boss' (Rank 0) using %d processes.\n\n", size);
        
        // --- THIS IS THE NEW PART ---
        printf("Generating %d random numbers to sort...\n", total_N);

        global_data.resize(total_N);
        srand(time(NULL)); // Seed for random numbers
        for (int i = 0; i < total_N; ++i) {
            global_data[i] = rand() % 5000; // Numbers from 0-4999
        }
        printf("...Done generating numbers.\n");
        // --- END OF NEW PART ---

        // We won't print the unsorted data... it's too long!
        // printArray("UNSORTED DATA: ", global_data);

        // --- SERIAL (SEQUENTIAL) SORT ---
        vector<int> serial_copy = global_data;
        
        printf("------------------------------------------------------\n");
        printf("SERIAL SORT: I (Rank 0) am sorting all %d numbers by myself...\n", total_N);
        
        serial_start_time = MPI_Wtime();
        
        // --- Run the serial sort ---
        mergeSort(serial_copy, 0, total_N - 1);
        
        serial_end_time = MPI_Wtime();
        serial_time = serial_end_time - serial_start_time;

        // We won't print the sorted data...
        // printArray("SERIALLY SORTED: ", serial_copy); 
        
        printf(">>> Serial (Sequential) Time: %f seconds <<<\n", serial_time);
        printf("------------------------------------------------------\n\n");
        
        printf("OK, now let's try it with the whole team (Parallel)!\n\n");
    }

    // --- 2. PREPARE FOR PARALLEL SORT (All processes do this) ---

    // Boss (Rank 0) needs to tell everyone else how big the array is.
    MPI_Bcast(&total_N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Everyone figures out their chunk size
    vector<int> send_counts(size);
    vector<int> displacements(size);
    int chunk_size = total_N / size;
    int remainder = total_N % size;
    int current_displacement = 0;

    for (int i = 0; i < size; ++i) {
        send_counts[i] = chunk_size;
        if (i < remainder) send_counts[i]++;
        displacements[i] = current_displacement;
        current_displacement += send_counts[i];
    }
    
    // Each process prepares its local data buffer
    int my_local_size = send_counts[rank];
    vector<int> local_data(my_local_size);

    // --- START PARALLEL TIMER ---
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        parallel_start_time = MPI_Wtime();
    }

    // --- 3. PARALLEL SORT (Distribution) ---
    MPI_Scatterv(
        (rank == 0) ? &global_data[0] : NULL,
        &send_counts[0],
        &displacements[0],
        MPI_INT,
        &local_data[0],
        my_local_size,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    // --- 4. PARALLEL SORT (Local Sorting) ---
    if (my_local_size > 0) {
        mergeSort(local_data, 0, my_local_size - 1);
    }

    // --- 5. PARALLEL SORT (The Merge Tournament) ---
    for (int step = 1; step < size; step = step * 2) {
        if (rank % (2 * step) == 0) { // Receiver
            int sender_rank = rank + step;
            if (sender_rank < size) {
                int received_size;
                MPI_Status status;
                MPI_Recv(&received_size, 1, MPI_INT, sender_rank, 0, MPI_COMM_WORLD, &status);
                
                vector<int> received_data(received_size);
                MPI_Recv(&received_data[0], received_size, MPI_INT, sender_rank, 0, MPI_COMM_WORLD, &status);

                // Manually merge...
                vector<int> merged_data;
                int i = 0, j = 0;
                while (i < my_local_size && j < received_size) {
                    if (local_data[i] <= received_data[j]) merged_data.push_back(local_data[i++]);
                    else merged_data.push_back(received_data[j++]);
                }
                while (i < my_local_size) merged_data.push_back(local_data[i++]);
                while (j < received_size) merged_data.push_back(received_data[j++]);

                local_data = merged_data;
                my_local_size = local_data.size();
            }
        } 
        else if (rank % step == 0) { // Sender
            int receiver_rank = rank - step;
            MPI_Send(&my_local_size, 1, MPI_INT, receiver_rank, 0, MPI_COMM_WORLD);
            MPI_Send(&local_data[0], my_local_size, MPI_INT, receiver_rank, 0, MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    // --- 6. FINAL RESULT & COMPARISON ---
    if (rank == 0) {
        // STOP THE PARALLEL CLOCK!
        parallel_end_time = MPI_Wtime();
        parallel_time = parallel_end_time - parallel_start_time;

        printf("------------------------------------------------------\n");
        printf("PARALLEL SORT: All %d processes finished the tournament!\n", size);
        
        // We won't print the sorted data...
        // printArray("PARALLEL SORTED: ", local_data);
        
        printf(">>> Parallel Time: %f seconds <<<\n", parallel_time);
        printf("------------------------------------------------------\n\n");

        
        // --- THE FINAL SHOWDOWN! ---
        printf("======================================================\n");
        printf("            FINAL TIME COMPARISON (N = %d)\n", total_N);
        printf("======================================================\n");
        printf("Serial Time (1 process):     %f seconds\n", serial_time);
        printf("Parallel Time (%d processes):  %f seconds\n", size, parallel_time);
        
        if (parallel_time < serial_time) {
            printf("\n>>> HOORAY! Parallel was %f seconds FASTER! <<<\n", serial_time - parallel_time);
        } else {
            printf("\n>>> HUH! Parallel was %f seconds SLOWER! <<<\n", parallel_time - serial_time);
            printf("(This is normal for small arrays! The 'talking' between\n");
            printf(" processes takes more time than just sorting the list!)\n");
        }
        printf("======================================================\n");
    }

    MPI_Finalize();
    return 0;
}


// Run Command : mpic++ 2-project_mpi.cpp -o a && mpirun -np 8 ./a
