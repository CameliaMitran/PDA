﻿#include "stdafx.h"
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h>
#include <time.h>
#define  PROCESSES 4
#define  N 100

using namespace std;

int main(int argc, char *argv[])
{
    int size, rank, number, leaderProcess;
    int source;
    int tag = 1;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);

    /*
    Generate random number.

    */
    number = rand() % 100 + 1;

    cout << "Process" << rank <<  "has the number " << number << "\n";

    /*
    Master process:

    */
    if (rank == 0)
    {
        int array[N], numberFromProcess, max = number, processID = 0;
        array[0] = number;

        /*
        The master process receives the number from each process.
        */
        for (int process = 0; process < PROCESSES; process++)
        {
         
            source = process;
            MPI_Recv(&numberFromProcess, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
            array[process] = numberFromProcess;

            cout << "Master received " << numberFromProcess << "from process " << process << "\n";
        }

        /*
        Find the process which has the greatest number - leaderProcess.
        */
        for (int process = 0; process < PROCESSES; process++) {
            if (array[process] >= max) {
                max = array[process];
                processID = process;
            }
        }

        leaderProcess = processID;

        /*
        Send the number from the leader process to the other processes.
        */
        for (int process = 0; process < PROCESSES; process++) {
            source = process;
            MPI_Send(&leaderProcess, 1, MPI_INT, process, tag, MPI_COMM_WORLD);

        }
        cout << "The leader process is process " << leaderProcess << ":\n";
    }
    /*
    This is a slave process which sends a number to the master process and receives the 
    number of the current leader process - process that generated the greatest number.
    */
    else if (rank != 0) {
        source = 0;

        MPI_Send(&number, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

        MPI_Recv(&leaderProcess, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
    }


    MPI_Finalize();

    return 0;
}
