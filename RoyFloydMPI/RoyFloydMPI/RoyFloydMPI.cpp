﻿#include "stdafx.h"
#include "stdio.h"
#include "mpi.h"
#include "stdlib.h"
#include "iostream"
using std::cout;
using std::endl;
#define MASTER 0
#define WORKTAG 1
#define DIETAG 2
#define n 300 /* number of nodes */
#define t 3
int dist[n][n]; /* dist[i][j] is the length of the edge between i and j if
                it exists, or 0 if it does not */

void printDist() {
    int i, j;
    cout << "    ";
    for (i = 0; i < n; ++i)
        cout<<'A' << i;
    cout << "\n";
    for (i = 0; i < n; ++i) {
        cout<<'A' << i;
        for (j = 0; j < n; ++j)
            cout<< dist[i][j];
        cout << "\n";
    }
    cout << "\n";
}

int main(int argc, char *argv[]) {

    int my_rank,                /* rank of process */
        num_processes,             
        slice,
        remaining = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    remaining = n % (num_processes - 1);
    slice = (n - remaining) / (num_processes - 1);

    /* master code */
    if (my_rank == MASTER) {
        double t1, t2;
        int disable = 0;
        int result[t];
        //initialize dist[][]
        int i, j;
        for (i = 0; i < n; ++i)
            for (j = 0; j < n; ++j)
                if (i == j)
                    dist[i][j] = 0;
                else
                    dist[i][j] = (int)(11.0 * rand() / (RAND_MAX + 1.0));// number creator 0-10

        printDist();// printout the new array
        t1 = MPI_Wtime();
        for (i = 1; i<num_processes; i++)
            MPI_Send(&dist, n*n, MPI_INT, i, WORKTAG, MPI_COMM_WORLD);//send the array dist in every machine

        do {
            MPI_Recv(&result, t, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == DIETAG)
                disable++;
            else
                if (dist[result[1]][result[2]]>result[0])
                    dist[result[1]][result[2]] = result[0];
        } while (disable < num_processes - 1);
        t2 = MPI_Wtime();
        printDist();// printout again the new array with shortest paths
        cout<<"total time "<< t2 - t1;
    }
    /* workers code*/
    else {

        int i, j, k;
        int out[t];
        MPI_Recv(&dist, n*n, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (my_rank + 1 != num_processes)
            remaining = 0;
        for (k = slice * (my_rank - 1); k < slice*(my_rank - 1) + slice + remaining; ++k)
            for (i = 0; i < n; ++i)
                for (j = 0; j < n; ++j)
                    /* If i and j are different nodes and if
                    the paths between i and k and between
                    k and j exist, do */
                    if ((dist[i][k] * dist[k][j] != 0) && (i != j))
                        /* See if you can't get a shorter path
                        between i and j by interspacing
                        k somewhere along the current
                        path */
                        if ((dist[i][k] + dist[k][j] < dist[i][j]) || (dist[i][j] == 0)) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            out[0] = dist[i][j];
                            out[1] = i;
                            out[2] = j;
                            MPI_Send(&out, t, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
                        }
        MPI_Send(0, 0, MPI_INT, MASTER, DIETAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
