/*
 ============================================================================
 Name        : m2m_serial.cpp
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Matrix matrix multiplication
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

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

void m2m() {
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1000; j++) {
			for (int k = 0; k < 1000; k++) {
				C[i][j] += A[i][k] * B[k][j];

			}
			printf("%.1f \n", C[i][j]);
		}
	}
}

int main(int argc, char *argv[]) {
	init_m();
	m2m();

	return 0;
}
