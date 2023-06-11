/*
 ============================================================================
 Name        : m2m_MPI.cpp
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include <math.h> 
#include "mpi.h" 
#include <iostream>
using namespace std;

double A[1000][1000];
double B[1000][1000];
double C[1000][1000];

void init_m() {
// initialise
	for (int i = 0; i < 1000; ++i) {
		for (int y = 0; y < 1000; ++y) {
			A[i][y] = rand();
			B[i][y] = rand();
		}
	}
}


int main(int argc, char *argv[]) {
	int n, rank, size, i, j, k;
	init_m();

	MPI::Init(argc, argv);
	size = MPI::COMM_WORLD.Get_size();
	rank = MPI::COMM_WORLD.Get_rank();

	n = 1000/size;
	MPI::COMM_WORLD.Bcast(B, 1000*1000, MPI::DOUBLE, 0);
	if (rank == 0)
		MPI::COMM_WORLD.Scatter(A, 1000*n, MPI::DOUBLE, MPI_IN_PLACE, 1000*n, MPI::DOUBLE, 0);
	else
		MPI::COMM_WORLD.Scatter(A, 1000*n, MPI::DOUBLE, A[rank*n], 1000*n, MPI::DOUBLE,  0);
	
	for (i = n*rank; i < n*(rank+1); i++) {
		for (j = 0; j < 1000; j++) {
			for (k = 0; k < 1000; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
			printf("%.1f \n", C[i][j]);
		}
	}
	if (rank == 0)
		MPI::COMM_WORLD.Gather(MPI_IN_PLACE, 1000*n, MPI::DOUBLE, C[n*rank], 1000*n, MPI::DOUBLE, 0);
	else
		MPI::COMM_WORLD.Gather(C[n*rank], 1000*n, MPI::DOUBLE, C, 1000*n, MPI::DOUBLE, 0);

	MPI::Finalize();
	return 0;
}

