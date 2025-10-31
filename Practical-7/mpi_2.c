#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    
    int my_rank;
    int count = 1; 
    int sendbuf = 100;  
    int recvbuf;  
    int tag = 0;  
    MPI_Comm comm = MPI_COMM_WORLD;  
    MPI_Status status;  
    
    MPI_Comm_rank(comm, &my_rank);
    
    if (my_rank == 0) {
        sendbuf = 100;  
        printf("Process %d: Sending %d to process 1\n", my_rank, sendbuf);
        MPI_Send(&sendbuf, count, MPI_INT, 1, tag, comm);
        
        MPI_Recv(&recvbuf, count, MPI_INT, 1, tag, comm, &status);
        printf("Process %d: Received %d from process 1\n", my_rank, recvbuf);
        
    } else if (my_rank == 1) {
        MPI_Recv(&recvbuf, count, MPI_INT, 0, tag, comm, &status);
        printf("Process %d: Received %d from process 0\n", my_rank, recvbuf);
        
        sendbuf = 200;  
        printf("Process %d: Sending %d to process 0\n", my_rank, sendbuf);
        MPI_Send(&sendbuf, count, MPI_INT, 0, tag, comm);
    }
    MPI_Finalize();
    return 0;
}

// Run: mpic++ mpi_2.c -o a && mpirun -np 2 ./a
// Source: https://github.com/kaityo256/deadlock