// HelloWMPI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <mpi.h>


void main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    std::cout << "Hello World" << std::endl;
    MPI_Finalize();
}
