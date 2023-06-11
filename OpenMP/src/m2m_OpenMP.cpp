/*
 ============================================================================
 Name        : m2m_OpenMP.cpp
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Matrix matrix multiplication in OpenMP
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double A[1000][1000];
double B[1000][1000];
double C[1000][1000];

void init_m() {
// initialise matrices
	for (int i = 0; i < 1000; ++i) {
		for (int y = 0; y < 1000; ++y) {
			A[i][y] = rand();
			B[i][y] = rand();
		}
	}
}

int main(int argc, char *argv[]) {
	int i, j, k;
	init_m();

#pragma omp parallel for private(i, j, k) shared(A,B,C)
	for (i = 0; i < 1000; i++) {
		for (j = 0; j < 1000; j++) {
			for (k = 0; k < 1000; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
			printf("%.1f \n", C[i][j]);
		}
	}
	return 0;
}
