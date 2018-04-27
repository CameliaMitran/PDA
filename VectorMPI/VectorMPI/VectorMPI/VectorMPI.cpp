
# include "stdafx.h"
# include "cstdlib"
# include "ctime"
# include "iomanip"
# include "iostream"
# include "mpi.h"
#include "time.h"

using namespace std;

/*
The master process computes numbers from 0 to 99 AND sends them to each worker process, which sums them.

*/
int main(int argc, char *argv[])
{
# define N 100

    double array[N];
    int i;
    int id;
    int p;
    double seed;
    MPI_Status status;
    double sum;
    double sum_all;
    int tag;
    MPI_Datatype MPI_MASTER;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &p);

    MPI_Comm_rank(MPI_COMM_WORLD, &id);


    /*
    The master process computes numbers from 0 to 99.

    */
    if (id == 0)
    {

        for (i = 0; i < N; i++)
        {
            array[i] = (double)i;
        }
    }

    /*
    The master process sends values to each worker process, which sums them.

    */
    MPI_Bcast(array, N, MPI_DOUBLE, MPI_MASTER, MPI_COMM_WORLD);
    /*
    Easch process does the sum for the values it received.

    */
    sum = 0.0;
    for (i = 0; i < N; i++)
    {
        sum = sum + array[i] * (double)id;
    }

    cout << "\n";
    cout << "SUM - Process " << id << ":\n";
    cout << " " << sum << "\n";

    /*
    Each worker process sends its sum back to the master process.

    */
    if (id != 0)
    {
        MPI_Send(&sum, 1, MPI_DOUBLE, MPI_MASTER, 1, MPI_COMM_WORLD);
    }
    else
    {
        sum_all = sum;
        for (i = 1; i < p; i++)
        {
            tag = 1;
            MPI_Recv(&sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1,
                MPI_COMM_WORLD, &status);

            sum_all = sum_all + sum;
        }
    }
    if (id == 0)
    {
        cout << "\n";
        cout << "SUM - Master process:\n";
        cout << "  The total sum is " << sum_all << "\n";
    }

    MPI_Finalize();

    return 0;

# undef N
}
