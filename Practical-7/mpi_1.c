#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv){

    MPI_Init(&argc, &argv);

    int P_rank, process_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &P_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_size);

    printf("Hello World!: process PID %d out of %d processes \n", P_rank, process_size);

    MPI_Finalize();
    
    return 0;
}


// Run:- mpic++ mpi_1.c -o a && mpirun --oversubscribe -np 10 ./a
// Source: https://curc.readthedocs.io/en/latest/programming/MPI-C.html#setup-and-hello-world