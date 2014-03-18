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

#define NMAX 100
using namespace std;

int main (int argc, char **argv) {

	ofstream fout(argv[3], ios::out | ios::trunc);
	int **m, **m2, **aux;
	int N, Nc;
	int S = atoi(argv[1]);
	int radius, i2, j2;
	int s;
	int maxDist;
	int i, j, k, cmax;

	// Citire matrice din fisier si alocare memorie:
	ifstream fin (argv[2]);
	if (fin.is_open()) {
		fin >> N;
		fin >> Nc;

		m = (int **)malloc(N * sizeof(int*));
		m2 = (int **)malloc(N * sizeof(int*));
		for (i = 0; i < N; i++) {
			m[i] = (int *)malloc(N * sizeof(int));
			m2[i] = (int *)malloc(N * sizeof(int));
			for (j = 0; j < N; j++) {
				fin >> m[i][j];
			}
		}
	}
	fin.close();

	int c[Nc]; // vector distante minime
	int culori_gasite; //cate culori am gasit
	int culori_cautate = Nc; // cate partide mai exista
	int nr_senatori[Nc]; // numar senatori in fiecare partid
	int culori_disparute[Nc]; // ce partid a disparut (1 => partidul nu mai exista)
	int fst, lst;

	// initializez vectorul, la inceput exista toate partidele
	for (k = 0; k < Nc; k++) {
		culori_disparute[k] = 0;
	}

	// pentru fiecare saptamana
	for (s = 0; s < S; s++) {

		// in fiecare sapt initializez cu 0 nr de senatori din fiecare partid
		for (k = 0; k < Nc; k++) {
			nr_senatori[k] = 0;
		}
			for (i = 0; i < N; i++) {
				for (j = 0; j < N; j++) {

					// initializez distanta minima cu 0
					for (k = 0; k < Nc; k++) {
						c[k] = 0;
					}
					radius = 1;
					culori_gasite = 0;
					maxDist = 0;
					while ((culori_gasite < culori_cautate) && (radius < N)) {
						// cazul 1
						i2 = i - radius;
						if (i2 >= 0 && i2 < N) {
							if (j - radius < 0) {
								fst = j;
							} else {
								fst = radius;
							}
							if (j + radius >= N) {
								lst = N - j - 1;
							} else {
								lst = radius;
							}
							for (k = fst; k >= -lst; k--) {

								j2 = j - k;
								// daca nu a fost gasita culoarea pana acuma
								if (c[m[i2][j2]] == 0) {
										c[m[i2][j2]] = radius;
										culori_gasite ++;
										maxDist = radius;
								}
							}
						}

						// cazul 2:
						i2 = i + radius;
						if (j - radius < 0) {
							fst = j;
						} else {
							fst = radius;
						}
						if (j + radius >= N) {
							lst = N - j - 1;
						} else {
							lst = radius;
						}
						if (i2 >= 0 && i2 < N ) {
							for (k = fst; k >= -lst; k--) {

								j2 = j - k;
								// daca nu a fost gasita culoarea pana acuma
								if (c[m[i2][j2]] == 0) {

									c[m[i2][j2]] = radius;
									culori_gasite ++;
									maxDist = radius;
								}
							}
						}

						// cazul 3:
						j2 = j - radius;
						if (i - radius < 0) {
							fst = i;
						} else {
							fst = radius;
						}
						if (i + radius >= N) {
							lst = N - i - 1;
						} else {
							lst = radius;
						}
						if (j2 >= 0 && j2 < N) {
							for (k = fst; k >= -lst; k--) {
								i2 = i - k;
								// daca nu a fost gasita culoarea pana acuma
								if (c[m[i2][j2]] == 0) {
										c[m[i2][j2]] = radius;
										culori_gasite ++;
										maxDist = radius;
								}
							}
						}

						// cazul 4:
						j2 = j + radius;
						if (i - radius < 0) {
							fst = i;
						} else {
							fst = radius;
						}
						if (i + radius >= N) {
							lst = N - i - 1;
						} else {
							lst = radius;
						}
						if (j2 >= 0 && j2 < N) {
							for (k = fst; k >= -lst; k--) {
								i2 = i - k;
								// daca nu a fost gasita culoarea pana acuma
								if (c[m[i2][j2]] == 0) {
										c[m[i2][j2]] = radius;
										culori_gasite ++;
										maxDist = radius;
								}
							}
						}


						radius ++;
					}// am terminat de calc dmin pt fiecare culoare

					// aleg maximul dintre C[k]
					cmax = 0;
					for (k = 0; k < Nc; k++) {
						if (c[k] == maxDist) {
							cmax = k;
							break;
						}
					}
					m2[i][j] = cmax; // noua culoare a senatorului (i,j)
					nr_senatori[cmax] ++; //nr cati senatori sunt de fiecare culoare
				}
			} // end for-each (i,j)


		// scriu in fisier cati senatori sunt in fiecare partid
		for (i = 0; i < Nc; i++) {
			fout << nr_senatori[i] << " ";
			// daca un partid e desfiintat, atunci nu mai caut culoarea respectiva
			if (nr_senatori[i] == 0 && culori_disparute[i] == 0) {
				culori_cautate --;
				culori_disparute[i] = 1;
			}
		}
		fout << endl;

		// inversez matricile:
		aux = m;
		m = m2;
		m2 = aux;


	}
	// end for (s=...)


	fout << endl;
	// scriu matricea finala in fisier
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
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






