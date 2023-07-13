/*
 * B_row_by_row.cpp
 *
 *  Created on: 06.07.2023
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
double tmpB[num][num];

MPI_Status status;

void init_m() {
// initialise
	for (int i = 0; i < num; ++i) {
		for (int y = 0; y < num; ++y) {
			A[i][y] = rand();
			B[i][y] = rand();
		}
	}
}

void rotate_b_matrix(){
	for (int i = 0; i < num; ++i) {
		for (int y = 0; y < num; ++y) {
			tmpB[num-1-y][i]=B[i][y];
		}
	}
	for (int i = 0; i < num; ++i) {
		for (int y = 0; y < num; ++y) {
			B[i][y]=tmpB[i][y];
		}
	}
}

int main(int argc, char *argv[]) {
	int n, rank, size, i, j, k, proc;
	init_m();
	rotate_b_matrix();

	MPI::Init(argc, argv);
	size = MPI::COMM_WORLD.Get_size();
	rank = MPI::COMM_WORLD.Get_rank();

	proc = size-1;
	while (num%proc!=0) {
		//if (rank==0) printf("Matrix size not divisible by number of processors\n");
		proc--;
	}
	int old_rank = rank;
	MPI_Comm new_comm;
	MPI_Comm_split(MPI_COMM_WORLD, old_rank<=proc, old_rank, &new_comm);
	int new_rank, new_size;
	MPI_Comm_rank(new_comm, &new_rank);
	MPI_Comm_size(new_comm, &new_size);
	MPI_Barrier(new_comm);

	n = num/proc;
	//printf("packet_size=%i here\n using %i slaves\n", n, proc);
	if (new_rank==0){
		for (int i = 1; i<=proc; i++){
			for (int j = 0; j<num; j++){
				for (int k = n*(i-1); k < n*i; k++) {
					MPI_Send(&B[num-1-j], num, MPI_DOUBLE, i, 0, new_comm);
				}
				MPI_Send(&A[j], num, MPI_DOUBLE, i, 0, new_comm);
			}
		}
	}
	if (new_rank<=proc && new_rank != 0){
		for (i = n*(new_rank-1); i < n*new_rank; i++) {
			MPI_Recv(&A[i], num, MPI_DOUBLE, 0, 0, new_comm, &status);
			for (j = 0; j < num; j++) {
				MPI_Recv(&B[num-1-j], num, MPI_DOUBLE, 0, 0, new_comm, &status);
				for (k = 0; k < num; k++) {
					C[i][j] += A[i][k] * B[num-1-j][k];
				}
				printf("%.1f \n", C[i][j]);
			}
		}
	}
	MPI::Finalize();
	return 0;
}

