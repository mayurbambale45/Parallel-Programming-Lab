#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data = rank; 
    int received_data;

    int left_neighbor = (rank - 1 + size) % size;
    int right_neighbor = (rank + 1) % size;

    int tag = 0;

    if (rank % 2 == 0) { 
        MPI_Send(&send_data, 1, MPI_INT, right_neighbor, tag, MPI_COMM_WORLD);
        MPI_Recv(&received_data, 1, MPI_INT, left_neighbor, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else { 
        MPI_Recv(&received_data, 1, MPI_INT, left_neighbor, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&send_data, 1, MPI_INT, right_neighbor, tag, MPI_COMM_WORLD);
    }

    printf("Process %d: Sent %d to %d, Received %d from %d\n", 
           rank, send_data, right_neighbor, received_data, left_neighbor);

    MPI_Finalize();
    return 0;
}

// Run: mpic++ -o a mpi_3.c  && mpirun -np 4 ./a
// Source: https://mpitutorial.com/tutorials/mpi-send-and-receive/