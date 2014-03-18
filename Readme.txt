Tema 1 APD

Nume: 			Ditu Alexandru Mihai
Grupa/Serie: 	333CA

Cuprins:
	1) Specificatii sistem
	2) Explicatii varianta seriala neoptimizata
	3) Explicatii varianta seriala optimizata
	4) Explicatii varianta paralela (am paralelizat varianta optimizata)
	5) Tabel cu analiza performantelor

1) Specificatii sistem
----------------------
	Sistem de operare: 	Linux Mint 15: Olivia (x86-64 bit)
	Kernel/Build:		3.8.0-19-generic / #29-Ubuntu SMP Wed Apr 17 18:16:28
						UTC 2013
	Processor:			Intel Core i7-2630QM @ 2.00 Ghz x 4 
						4 Core-uri
						8 Thread-uri
						Max Turbo Frequency - 2.9 Ghz
	Cache Memory:		6 MB
	Compilator:			g++ (Ubuntu/Linaro 4.7.3-1ubuntu1) 4.7.3

2) Explicatii varianta seriala neoptimizata
-------------------------------------------
	Initial citesc datele de intrare din fisier (variabilele folosite sunt
explicate/comentate in sursa, nu le mai reiau aici, dar in principiu folosesc
2 matrici, una pentru saptamana curenta "m" si una pentru saptamana urmatoare
"m2").
	In fiecare saptamana calculez pentru fiecare senator (i,j) distanta maxima
dintre distantele minime la fiecare culoare. Pentru aceasta am nevoie de inca
un for (i2,j2) in care iterez iar prin toata matricea, pentru a putea calcula
distanta minima de la (i,j) la (i2,j2) pentru fiecare culoare, pe care o retin
intr-un vector c[Nc]. Astfel pentru fiecare (i2,j2) (cu i != i2 si j != j2),
folosind formula c[culoare] = MAX(abs(i-i2), abs(j-j2)) (folosesc std::abs din
math.h) retin distanta minima de la (i,j) la fiecare culoare.
	Dupa ce termin de iterat prin toata matricea (cu i2,j2), noua culoare a
senatorului (i,j) devine MAX(dintre c[]), adica ia valoarea maxima dintre
distantele minime. Aceasta valoare o retin in alta matrice, m2 iar dupa ce
termin de iterat cu (i,j):
	- calculez nr de senatori din fiecare partid
	- interschimb matricile intre ele (m cu m2)
	- scriu in fisier nr de senatori din fiecare partid
	In final, dupa ce repet algoritmul pentru fiecare saptamana scriu si 
matricea finala in fisier.
	Deoarece am 4 for-uri imbricate, fiecare mergand de la 0 la N-1 => 
complexitatea algoritmului este O(N^4).

Durata executie (pe testul cu: S=100, N=100, Nc=20): 2m38.973s

3) Explicatii varianta seriala optimizata
-----------------------------------------
	Trebuie sa recunosc ca am avut ceva batai de cap pana sa reusesc sa
scot timpul cel mai bun.
	Diferenta majora intre cei doi algorimti este ca nu mai iterez din nou
prin toata matricea, pentru fiecare senator, deorece este inutil, ci caut doar
cat este nevoie: folosesc "cercuri concentrice" in jurul fiecarui senator.
Astfel, pentru fiecare senator i,j:
	- initializez cu 0, distanta minima la fiecare culoare c[k], cu 0<=k<Nc
	- initializez raza cu 1 si nr de culori gasite cu 0 (de aici provine
eficienta algorimului, prin faptul ca nu mai iterez prin toata matricea, ci
doar cat timp nu am gasit cate un "reprezentant" din fiecare culoare/partid,
deorece stiu ca atunci cand i-am gasit pe toti sigur am obtinut si valoarea
minima - de asta folosesc cercuri concentrice -); in plus, nu mai e nevoie nici
sa mai calculez distanta intre cei 2 senatori, deorece aceasta este egala cu
raza;
	- astfel, cat timp nr de culori gasite e mai mic decat numarul de culori
cautate si cat timp nu am iesit din matrice, pentru fiecare culoare gasita,
daca este cazul, actualizez distanta minima. In plus, pastrez si distana 
maxima gasita.
	- pentru cercurile concentrice am folosit 4 for-uri, cate unul pentru
fiecare latura; problema (se consuma prea mult timp) pe care am intalnit-o
aici a fost ca, initial ma intrebam in fiecare for daca nu ies din matrice;
acest lucru s-a dovedit a fi foarte costisitor, asa ca am calculat, inainte
de for, ce valori trebuia sa ia variabila (k, in cazul meu) pentru a nu iesi
din matrice; astfel timpul de executie (pentru testul cu S=100, N=100, Nc=20)
a scazut cu vreo 7 secunde.
	- dupa ce am calculat distanta minima pentru fiecare culoare, noul partid
pentru senatorul (i,j) devine max dintre culorile minime (la fel ca la
algoritmul neoptimizat), numai acum stiu deja distMax, si efectiv ma opresc la
prima culoare ce are dist == distMax (elimin cazul de egalitate).
	- creste si nr de senatori din culoarea noua obtinuta
	- dupa ce am terminat calculele pentru toti senatorii, scriu in fiser
nr de senatori din fiecare partid si verific si daca a disparut vreo culoare,
si atunci scad nr_culori_cautate.
	- apoi inversez matricile intre ele (m cu m2)
	- algoritmul se repeta in saptamana urmatoare
	- la final scriu matricea in fisier

Durata executie (pe testul cu: S=100, N=100, Nc=20): 0m17.872s (pe masina mea)

4) Explicatii varianta paralela
-------------------------------
	Aici am decis sa paralelizez doar varianta optimizata (nu am mai paralelizat
si cealalata varianta deoarece era aproape acelasi lucru si nu am mai vazut
rostul).
	Ideea mea este urmaotarea: deorece calculul culorii fiecarui senator
este indepenta de ceilalti senatori am decis ca fiecare thread sa primeasca
cate un nr de senatori (adica cate o linie din matrice) si sa calculeze
pentru fiecare in parte noua culoare politica.
	Astfel am folosit comanda:
	#pragma omp parallel for \
			shared(m, m2, aux, nr_senatori, N, Nc) \
			private(radius, culori_gasite, maxDist, i, j, i2, j2, k, c, fst, lst, cmax) \
			schedule(runtime)
inainte de for-ul corespunzator fiecarei linii din matrice (adica for(i=0..))
	Variabilele shared sunt:
	- matricea curenta "m" si matricea din saptamana urmatoare "m2" si
	  "aux" folosit pentru interschimbarea saptamanala de matrici
	- N, Nc
	- si un vector "nr_senatori" -> in aceasta variabila numar in fiecare
	  saptamana cati senatori sunt din fiecare culoare; problema aici este ca
	  deorece folosesc mai multe thread-uri e posbil ca aceastea sa scrie in
	  acelasi timp, asa ca am mai folosit o directiva: #pragma omp critical, pentru
	  a evita aceasta situatie
	- private sunt toate celalte variabile utilizate in algoritmul serial
	  optimizat
	In rest, algoritmul este acealasi cu cel serial optimizat.

	Mentionez ca schedule este setat la runtime.
	Durata executie (pe testul cu: S=100, N=100, Nc=20):

5) Tabel cu analiza performantelor
----------------------------------
Timp exectuie serial optimizat(s): 17.872

S	N	Nc	Nr.Thread-uri	Timp(s)	Speed-up	Tip		Chunksize	

100	100	20	2				9.991	1.79		Static	10	
100	100	20	4				6.345	2.82		Static	10	
100	100	20	6				6.192	2.89		Static	10	
100	100	20	8				6.251	2.86		Static	10	
								
100	100	20	2				9.884	1.81		Dinamic	10	
100	100	20	4				6.14	2.91		Dinamic	10	
100	100	20	6				6.205	2.88		Dinamic	10	
100	100	20	8				6.111	2.92		Dinamic	10	
								
100	100	20	2				10.198	1.75		Static	25	
100	100	20	4				8.33	2.15		Static	25	
100	100	20	6				6.216	2.88		Static	25	
100	100	20	8				6.281	2.85		Static	25	
								
100	100	20	2				10.29	1.74		Dinamic	25	
100	100	20	4				6.219	2.87		Dinamic	25	
100	100	20	6				6.46	2.77		Dinamic	25	
100	100	20	8				6.924	2.58		Dinamic	25	
