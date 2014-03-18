/*
 * serial.cpp
 *
 *  Created on: Nov 2, 2013
 *      Author: Ditu Alexandru 333 CA
 */
#include <iostream>
#include <fstream>
#include <omp.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
using namespace std;

int** readMatrix (char *filename, int &N, int &Nc) {
	ifstream fin (filename);
	int **m;
	if (fin.is_open()) {
		fin >> N;
		fin >> Nc;
		m = (int **)(malloc (N * sizeof(int *)));

		for (int i = 0; i < N; i++) {
			m[i] = (int*)malloc(N * sizeof(int));
			for (int j = 0; j < N; j++) {
				fin >> m[i][j];
			}
		}
	}
	fin.close();
	return m;
}

void showMatrix (int **m, int N) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
}

void writeMatrix (int **m, int N, ofstream fout) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			fout << m[i][j] << " ";
		}
		fout << endl;
	}
}



int main (int argc, char **argv) {
	int **m, **m2;
	int N, Nc;
	int *c; // vector culori partide politice
	ofstream fout(argv[3], ios::out | ios::trunc);
	int S = atoi(argv[1]);

	m = readMatrix(argv[2], N, Nc);

	// alocare memorie pentru m2 si c
	m2 = (int**)malloc(N * sizeof(int*));
	for (int i = 0; i < N; i++) {
		m2[i] = (int*)malloc(N * sizeof(int));
	}
	c = (int*) malloc(Nc * sizeof(int));

	int d1, d2, max;
	for (int s = 0; s < S; s++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {

				for (int k = 0; k < Nc; k++) {
					c[k] = 0;
				}

				for (int i2 = 0; i2 < N; i2++) {
					for (int j2 = 0; j2 < N; j2++) {
						if (i != i2 || j != j2) {
							// calculez distanta de la (i,j) la (i2,j2)
							d1 = std::abs(i - i2);
							d2 = std::abs(j - j2);
							if (d1 > d2) {
								max = d1;
							} else {
								max = d2;
							}

							// c[k] = dist_min
							if(c[m[i2][j2]] == 0) {
								c[m[i2][j2]] = max;
							} else {
								if (max < c[m[i2][j2]]) {
									c[m[i2][j2]] = max;
								}
							}
						}
					}
				}

				// aleg maximul dintre C[k]
				int dmax = c[0];
				int cmax = 0;
				for (int k = 0; k < Nc; k++) {
					if (c[k] > dmax) {
						dmax = c[k];
						cmax = k;
					}
				}
				m2[i][j] = cmax;
			}
		}

		// numar cati senatori sunt in fiecare partid
		for (int i = 0; i < Nc; i++) {
			c[i] = 0;
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				c[m2[i][j]]++;
			}
		}

		for (int i = 0; i < Nc; i++) {
			fout << c[i] << " ";
		}
		fout << endl;

		// inversez matricile:
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				m[i][j] = m2[i][j];
			}
		}
	}
	// scriu matricea finala in fisier
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			fout << m[i][j] << " ";
		}
		fout << endl;
		free(m[i]);
		free(m2[i]);
	}

	fout.close();
	free(m);
	free(m2);
	return 0;
}



