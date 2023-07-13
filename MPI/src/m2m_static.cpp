/*
 ============================================================================
 Name        : MPI_test.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Multiply two 1000x1000 matrices
 ============================================================================
 */
#include "mpi.h" 
#include <iostream>
using namespace std;

#define num 1000
#define USE_MPI_IN_PLACE 0

double A[num][num];
double B[num][num];
double C[num][num];

void init_m() {
// initialise
	for (int i = 0; i < num; ++i) {
		for (int y = 0; y < num; ++y) {
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

	if (num%size!=0) {
		if (rank==0) printf("Matrix size not divisible by number of processors\n");
		MPI_Finalize();
		exit(-1);
	}

	n = num/size;
	MPI::COMM_WORLD.Bcast(B, num*num, MPI::DOUBLE, 0);
	if (rank == 0)
		MPI::COMM_WORLD.Scatter(A, num*n, MPI::DOUBLE, MPI_IN_PLACE, num*n, MPI::DOUBLE, 0);
	else
		MPI::COMM_WORLD.Scatter(A, num*n, MPI::DOUBLE, A[rank*n], num*n, MPI::DOUBLE,  0);
	
	for (i = n*rank; i < n*(rank+1); i++) {
		for (j = 0; j < num; j++) {
			for (k = 0; k < num; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
			printf("%.1f \n", C[i][j]);
		}
	}
	if (rank == 0)
		MPI::COMM_WORLD.Gather(MPI_IN_PLACE, num*n, MPI::DOUBLE, C[n*rank], num*n, MPI::DOUBLE, 0);
	else
		MPI::COMM_WORLD.Gather(C[n*rank], num*n, MPI::DOUBLE, C, num*n, MPI::DOUBLE, 0);

	MPI::Finalize();
	return 0;
}

