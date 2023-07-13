/*
 * Dynamic_proc_count.cpp
 *
 *  Created on: 11.07.2023
 *      Author: root
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
	int n, rank, size, i, j, k, proc;
	init_m();
	MPI::Init(argc, argv);
	size = MPI::COMM_WORLD.Get_size();
	rank = MPI::COMM_WORLD.Get_rank();

	proc = size;
	while (num%proc!=0) {
		//if (rank==0) printf("Matrix size not divisible by number of processors\n");
		proc--;
	}
	int old_rank = rank;
		MPI_Comm new_comm;
		MPI_Comm_split(MPI_COMM_WORLD, old_rank<proc, old_rank, &new_comm);
		int new_rank, new_size;
		MPI_Comm_rank(new_comm, &new_rank);
		MPI_Comm_size(new_comm, &new_size);
		MPI_Barrier(new_comm);
		//if (rank==0&&proc!=size)
		//printf("Matrix cannot be divided by specified process count. Continuing with %i processes instead.\n", new_size);

		//printf("#%i here\n", old_rank);
		n = num/proc;
		MPI_Bcast(B, num*num, MPI::DOUBLE, 0, new_comm);
		if (old_rank<proc){
			if (new_rank == 0)
				MPI_Scatter(A, num*n, MPI::DOUBLE, MPI_IN_PLACE, num*n, MPI::DOUBLE, 0, new_comm);
			else{
				MPI_Scatter(A, num*n, MPI::DOUBLE, A[new_rank*n], num*n, MPI::DOUBLE,  0, new_comm);
			}
			for (i = n*new_rank; i < n*(new_rank+1); i++) {
				for (j = 0; j < num; j++) {
					for (k = 0; k < num; k++) {
						C[i][j] += A[i][k] * B[k][j];
					}
					printf("%.1f \n", C[i][j]);
				}
			}
		}
		if (new_rank<proc){
			if (new_rank == 0)
				MPI_Gather(MPI_IN_PLACE, num*n, MPI::DOUBLE, C[n*new_rank], num*n, MPI::DOUBLE, 0, new_comm);
			else
				MPI_Gather(C[n*new_rank], num*n, MPI::DOUBLE, C, num*n, MPI::DOUBLE, 0, new_comm);
		}
		//printf("#%i done \n", rank);
	MPI::Finalize();
	return 0;
}





