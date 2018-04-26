
# include "stdafx.h"
# include "cstdlib"
# include "ctime"
# include "iomanip"
# include "iostream"
# include "mpi.h"

#include "time.h"
using namespace std;

int main(int argc, char *argv[]);
void timestamp();
int main(int argc, char *argv[])
{
# define N 100

    double array[N];
    int i;
    int id;
    int p;
    double PI = 3.141592653589793238462643;
    double seed;
    MPI_Status status;
    double sum;
    double sum_all;
    int tag;
    MPI_Datatype master;
    //
    //  Initialize MPI.
    //
    MPI_Init(&argc, &argv);
    //
    //  Get the number of processes.
    //
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    //
    //  Determine the rank of this process.
    //
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    //
    //  Say hello.
    //
    if (id == 0)
    {
        timestamp();
        cout << "\n";
        cout << "SUM - Master process:\n";
        cout << "  The master process computes some coefficients,\n";
        cout << "  sends them to each worker process, which sums them.\n";
        cout << "\n";
        cout << "  Compiled on " << __DATE__ << " at " << __TIME__ << ".\n";
        cout << "\n";
        cout << "  The number of processes available is " << p << "\n";
    }
    //
    //  The master process initializes the array.
    //
    if (id == 0)
    {
        seed = 1.2345;

        for (i = 0; i < N; i++)
        {
            array[i] = (double)i * seed * PI;
        }
    }

    //  The master process broadcasts the computed initial values 
    //  to all the other processes.
    MPI_Bcast(array, N, MPI_DOUBLE, master, MPI_COMM_WORLD);

    //  Each process adds up its entries.
    sum = 0.0;
    for (i = 0; i < N; i++)
    {
        sum = sum + array[i] * (double)id;
    }

    cout << "\n";
    cout << "SUM - Process " << id << ":\n";
    cout << "  My contribution to the sum is " << sum << "\n";
    // 
    //  Each worker process sends its sum back to the master process.
    //
    if (id != 0)
    {
        MPI_Send(&sum, 1, MPI_DOUBLE, master, 1, MPI_COMM_WORLD);
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
    //
    //  Terminate MPI.
    //
    MPI_Finalize();
    //
    //  Terminate.
    //
    if (id == 0)
    {
        cout << "\n";
        cout << "SUM - Master process:\n";
        cout << "  Normal end of execution.\n";
        cout << "\n";
        timestamp();
    }
    return 0;

# undef N
}

void timestamp()
{
# define TIME_SIZE 40

    static char time_buffer[TIME_SIZE];
    const struct std::tm *tm_ptr;
    size_t len;
    std::time_t now;

    now = std::time(NULL);
    tm_ptr = std::localtime(&now);

    len = std::strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr);

    std::cout << time_buffer << "\n";

    return;
# undef TIME_SIZE
}