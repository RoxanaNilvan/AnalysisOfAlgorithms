/* Nilvan Roxana Maria 
   grupa 30228
*  Am implementat 3 variante de sortare pentru siruri de numere
*  Prima varanta este sortarea prin selectie
*  A doua varianta este sortearea prin inserarea
*  A treia varianta este sortarea bulelor
* 
*  Dupa implementarea celor 3 algoritmi si trasarea graficelor am constatat ca in caz mediu statistic cele mai putine atribuiri sunt realizate de metoda de sortare prin 
*  selectie, urmata de metoda de sortare prin insertie, iar in cele din urma metoda de sortare a bulelor. Din punct de vedere al comparatiilor, cele mai putine comparatii
*  sunt realizate metoda de sortare prin insertie, urmata de sortare prin selectie si metoda de sortare a bulelor. Per total cel mai rapid algoritm din punct 
*  de vedere al oparatiilor realizate este cel de sortare prin selectie, urmat de sortarea prin insertie, iar in cele din urma metoda de sortare a bulelor.
* 
*  In caz favorabil, ata metoda de sortare prin selectie cat si metoda de sortare a bulelor nu realizeaza nici o atribuire, in timp ce metoda de sortare prin insertie face 
*  atribuiri. Din punct de vedere al comparatiilor, metoda de sortare prin insertie nu realizeaza nici o comparatie, in timp ce celalate doua realizeaza un numar egal de 
*  comparatii. Per toatl, cel mai rapid algoritm in caz favorabil este cel de sortare prin insertie.
* 
*  In caz defavorabil cele mai putine atribuiri sunt realizate de metoda de sortare prin selectie, mai apoi de metoda de sortare prin insertie si la final metoda de sortare 
*  a bulelor. Din punct de vedere al comparatiilor toate cele trei metode realizeaza un numar egal de comparatii. Per toatl cel mai rapid algoritm este cel de sortare prin
*  selectie, urmat de cel de sortare prin insertie, iar cel mai lent este cel de sortare a bulelor.
* 
*  Din masuratori ne putem da seama ca cel mai bun algoritm este cel de sortare prin selectie, fiind cel mai rapid atat in caz mediu statistic cat si in caz defavorabil.
*/
#include<stdio.h>
#include "Profiler.h"

Profiler p("heapAlgorithms");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

void selection_sort(int vector[], int lenght) {
	Operation opCompS = p.createOperation("selection.comp", lenght);
	Operation opAtrS = p.createOperation("selection.atr", lenght);

	int min_poz;
	for (int i = 0; i < lenght - 1; i++) {
		min_poz = i;
		for (int j = i + 1; j < lenght; j++) {
			opCompS.count();
			if (vector[j] < vector[min_poz]) {
				min_poz = j;
			}
		}

		if (i != min_poz) {
			opAtrS.count(3);
			int aux = vector[i];
			vector[i] = vector[min_poz];
			vector[min_poz] = aux;
		}
	}
}

void insertion_sort(int vector[], int lenght) {
	Operation opCompI = p.createOperation("insertion.comp", lenght);
	Operation opAtrI = p.createOperation("insertion.atr", lenght);
	for (int i = 1; i < lenght; i++) {
		opAtrI.count();
		int key = vector[i];
		int j = i - 1;
		while (vector[j] > key && j >= 0) {
			opCompI.count();
			opAtrI.count();
			vector[j + 1] = vector[j];
			j = j - 1;
		}
		opAtrI.count();
		vector[j + 1] = key;
	}
}

void bubblesort(int vector[], int lenght) {
	Operation opCompB = p.createOperation("bubble.comp", lenght);
	Operation opAtrB = p.createOperation("bubble.atr", lenght);
	for (int i = 0; i < lenght - 1; i++) {
		for (int j = lenght - 1; j > i; j--) {
			opCompB.count();
			if (vector[j] < vector[j - 1]) {
				opAtrB.count(3);
				int aux = vector[j];
				vector[j] = vector[j - 1];
				vector[j - 1] = aux;
			}
		}
	}
}

void generate_worst_case(int v[], int n) {
	int i = n;
	int j = i - 1;
	int last_value = v[n - 1];
	while (j >= 0 && i >= 0) {
		v[i] = v[j];
		i--;
		j--;
	}
	v[0] = last_value;
}


void demo() {
	int s[] = { 7, 2, 8, 5, 9, 1, 6 };
	int n = sizeof(s) / sizeof(s[0]);

	int i[] = { 7, 2, 8, 5, 9, 1, 6 };
	int m = sizeof(i) / sizeof(i[0]);

	int b[] = { 7, 2, 8, 5, 9, 1, 6 };
	int p = sizeof(b) / sizeof(b[0]);

	selection_sort(s, n);
	insertion_sort(i, m);
	bubblesort(b, p);

	for (int i = 0; i < n; i++) {
		printf("%d ", s[i]);
	}

	printf("\n");

	for (int j = 0; j < m; j++) {
		printf("%d ", i[j]);
	}

	printf("\n");

	for (int i = 0; i < p; i++) {
		printf("%d ", b[i]);
	}
}

void tests_average(int order) {
	int v[MAX_SIZE], v2[MAX_SIZE], v3[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		for (int test = 0; test < NR_TESTS; test++) {
			FillRandomArray(v, n, 10, 50000, false, order);
			for (int i = 0; i < n; i++) {
				v2[i] = v[i];
				v3[i] = v[i];
			}

			insertion_sort(v, n);
			selection_sort(v2, n);
			bubblesort(v3, n);
		}
	}
	p.divideValues("insertion.atr", NR_TESTS);
    p.divideValues("insertion.comp", NR_TESTS);
	p.addSeries("insertion", "insertion.atr", "insertion.comp");
	

	p.divideValues("selection.atr", NR_TESTS);
   	p.divideValues("selection.comp", NR_TESTS);
	p.addSeries("selection", "selection.atr", "selection.comp");
	

	p.divideValues("bubble.atr", NR_TESTS);
	p.divideValues("bubble.comp", NR_TESTS);
	p.addSeries("bubble", "bubble.atr", "bubble.comp");

	p.createGroup("average_case_atr", "insertion.atr", "selection.atr", "bubble.atr");
	p.createGroup("average_casse_comp", "insertion.comp", "selection.comp", "bubble.comp");
	p.createGroup("average_case_total", "insertion", "selection", "bubble");

}

void test_2() {
	tests_average(UNSORTED);
	p.showReport();
	
}
void test_best(int order) {
	int v[MAX_SIZE], v2[MAX_SIZE], v3[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
			FillRandomArray(v, n, 10, 50000, false, order);
			for (int i = 0; i < n; i++) {
				v2[i] = v[i];
				v3[i] = v[i];
			}
			insertion_sort(v, n);
			selection_sort(v2, n);
			bubblesort(v3, n);
		}
	p.addSeries("insertion", "insertion.atr", "insertion.comp");
	p.addSeries("selection", "selection.atr", "selection.comp");
	p.addSeries("bubble", "bubble.atr", "bubble.comp");

	p.createGroup("best_case_atr", "insertion.atr", "selection.atr", "bubble.atr");
	p.createGroup("best_casse_comp", "insertion.comp", "selection.comp", "bubble.comp");
	p.createGroup("best_case_total", "insertion", "selection", "bubble");

	p.showReport();
	}
	
void test_1() {
	test_best(ASCENDING);
}

void test_worst() {
	int v[MAX_SIZE], v2[MAX_SIZE], v3[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		FillRandomArray(v, n, 10, 50000, false, ASCENDING);
		
		// worst selection
		generate_worst_case(v, n);

		selection_sort(v, n);

		FillRandomArray(v2, n, 10, 50000, false, DESCENDING);
		for (int i = 0; i < n; i++) {
			v3[i] = v2[i];
		}

		insertion_sort(v2, n);
		bubblesort(v3, n);

	}
	p.addSeries("insertion", "insertion.atr", "insertion.comp");
	p.addSeries("selection", "selection.atr", "selection.comp");
	p.addSeries("bubble", "bubble.atr", "bubble.comp");

	p.createGroup("worst_case_atr", "insertion.atr", "selection.atr", "bubble.atr");
	p.createGroup("worst_casse_comp", "insertion.comp", "selection.comp", "bubble.comp");
	p.createGroup("worst_case_total", "insertion", "selection", "bubble");

	p.showReport();
}
void test_3() {
	test_worst();
}

int main() {
	//demo();
	//test_1();
	//test_2();
    test_3();
	return 0;
}