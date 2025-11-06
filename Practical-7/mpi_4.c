#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int n = 10;
    int array[10];
    int local_sum = 0;
    int total_sum = 0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size != 2) {
        if (rank == 0) {
            printf("Error: This program requires exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    if (rank == 0) {
        printf("Process 0: Initializing array...\n");
        for (int i = 0; i < n; i++) {
            array[i] = i + 1;
        }
        
        for (int i = 0; i < n/2; i++) {
            local_sum += array[i];
        }
        printf("Process 0: Local sum (first half) = %d\n", local_sum);
        
        MPI_Send(&array[n/2], n/2, MPI_INT, 1, 0, MPI_COMM_WORLD);
        
        int p1_sum;
        MPI_Recv(&p1_sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        total_sum = local_sum + p1_sum;
        printf("Process 0: Total sum = %d\n", total_sum);
        
    } else {
        int recv_array[5];
        
        MPI_Recv(recv_array, n/2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        for (int i = 0; i < n/2; i++) {
            local_sum += recv_array[i];
        }
        printf("Process 1: Local sum (second half) = %d\n", local_sum);
        
        MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}

// Run:  mpic++ -o a mpi_4.c && mpirun -np 4 ./a
// Source: https://stackoverflow.com/questions/16162796/write-mpi-program-sum-in-a-the-array-with-two-process