/*Nilvan Roxana Maria
* Grupa 30228
* In aceasta tema am implementat o structura de hash table pentru a evalua eforul de cautare a unei chei.
* Pentru a evalua efortul am flosit urmatoarea strategie:
* - am umplut cu un diferit factor de umplere : 80%, 85%, 90%, 95%, 99% si am aplicat operatia de cautare pe fiecare dintre aceste valori
* si am realizat un numar de 5 teste pentru a calcula efortul mediu statistic 
*  Din masuratorile efectuate am observat ca efortul de cautare atat a unei valori ce se afla in tabela cat si a unei valori ce nu se afla in
* tabela devine din ce in ce mai mare cu cat tabela este mai populata
* Pentru a realiza masuratorile am flosit aceasta stategie:
* - am generat un vector aleator de valori ce vor fi introduse in tabela
* - am genetat un tabel aleator de pozitii din tabela pentru a fi siguri ca valorile sunt imaprtite uniform pe marimea tabelei, si am aplicat
* algoritmul de cauatre pe elementul din vector de pe pozitia aleator generata("array[searchArray[i]]")
* - pentu a ne asigura ca, cautam valori mai ce nu sunt in tabela, am generat un vector de valroi mai mari decat cea mai mare valoare introdusa
* in tabela
* O alta masurataore pe care am realizat-o este cea de cautare dupa stergere. Pentru a realiza aceasta masuratoare am procedat in felul urmator:
* am generat un vector aleator de chei ce vor fi intorduse in tabela, porind de la un factor de umplere de 99%, iar mai apoi un vectoraleator 
* de pozitii ce vor fi sterse din tabela pentru a ajunge la un factor de umplere de 80%. Am parcurs vectorul de pozitii stergand in acelasi
* timp elementul atat din tabela cat si din vectorul de valori. Iar mai apoi am aplicat strategia decrisa mai sus.
*/
#include<stdio.h>
#include<stdlib.h>
#include "Profiler.h"

Profiler p("tabeleDeDispersie");

#define NR_TESTS 5

typedef struct Node {
	int key;
	int value;
	int deffined;
}Node;

typedef struct IntHashTable {
	int size;
	Node* nodes;
}IntHashTabel;

IntHashTable* intHashTableNew(unsigned hashTableSize) {
	IntHashTable* hashTable = (IntHashTable*)calloc(hashTableSize, sizeof(IntHashTabel));
	hashTable->size = hashTableSize;
	hashTable->nodes = (Node*)calloc(hashTable->size, sizeof(Node));
	return hashTable;
}

void intHashTableFree(IntHashTabel *hashTable) {
	free(hashTable);
}

int hashFunction(int key, int size, int i) {
	return (key + key * i + key * i * i) % size;
}

void intHashTableInsert(IntHashTable* hashTable, int key, int value) {
	for (int i = 0; i < hashTable->size; i++) {
		int newIndex = hashFunction(key, hashTable->size, i);
		if (hashTable->nodes[newIndex].deffined == 0) {
			hashTable->nodes[newIndex].key = key;
			hashTable->nodes[newIndex].value = value;
			hashTable->nodes[newIndex].deffined = 1;
			return;
		}
	}
}

int hashTableSearch(IntHashTabel* hashTable, int key, float *effort) {
	for (int i = 0; i < hashTable->size; i++) {
		(*effort)++;
		int newIndex = hashFunction(key, hashTable->size, i);

		if (hashTable->nodes[newIndex].key == key) {
			return newIndex;
		}
		else if (hashTable->nodes[newIndex].deffined == 0) {
			return 0;
		}
	}

	return 0;
}

void intHashTableDelete(IntHashTabel* hashTable, int key) {
	for (int i = 0; i < hashTable->size; i++) {
		if (hashTable->nodes[i].key == key) {
			if (hashTable->nodes[i].deffined == 1) {
				hashTable->nodes[i].value = 0;
				hashTable->nodes[i].deffined = 0;
			}
		}
	}
}

void intHashTablePrint(IntHashTable* hashTable) {
	for (int i = 0; i < hashTable->size; i++) {
		if (hashTable->nodes[i].deffined == 1) {
			printf("( %d, %d ) \n", hashTable->nodes[i].key, hashTable->nodes[i].value);
		}
		else {
			printf("not occupaied\n");
		}
	}
}

void demo() {
	IntHashTabel* hashTable = intHashTableNew(10);
	intHashTableInsert(hashTable, 2, 4);
	intHashTableInsert(hashTable, 5, 100);
	intHashTableInsert(hashTable, 15, 32);
	intHashTableInsert(hashTable, 9, 20);
	intHashTableInsert(hashTable, 3, 2);
	intHashTableInsert(hashTable, 13, 6);
	intHashTableInsert(hashTable, 4, 9);
	intHashTableInsert(hashTable, 7, 2);
	intHashTableInsert(hashTable, 14, 21);
	intHashTablePrint(hashTable);

	float effort = 0;
	int var2 = hashTableSearch(hashTable, 4, &effort);
	printf("\n %d \n", var2);
	printf("Table after delete \n");
	intHashTableDelete(hashTable, 3);
	intHashTablePrint(hashTable);
}

void demoBiggerValues() {
	IntHashTabel* hashTable = intHashTableNew(100);
	int array[95];
	FillRandomArray(array, 95, 10, 1000, false, UNSORTED);
	for (int i = 0; i < 95; i++) {
		intHashTableInsert(hashTable, array[i], array[i]);
	}
	intHashTablePrint(hashTable);

	float effort = 0;
	int var2 = hashTableSearch(hashTable, array[47], &effort);
	printf("\n %d \n", var2);

	printf("Table after delete \n");
	intHashTableDelete(hashTable, array[56]);
	intHashTableDelete(hashTable, array[20]);
	intHashTableDelete(hashTable, array[32]);
	intHashTablePrint(hashTable);
}

void testEffort(int factor) {
	int n = 9973;
	IntHashTable* hashTable = intHashTableNew(n);

	int array[9973];
	FillRandomArray(array, factor, 10, 25000, false, UNSORTED);
	for (int i = 0; i < factor; i++) {
		intHashTableInsert(hashTable, array[i], array[i]);
	}
	float medFound = 0, medMaxFound = 0;
	float medNotFound = 0, medMaxNotFound = 0;

	for (int test = 0; test < 5; test++) {
		int numberOfSearchedValues = 3000;

		float averageFound = 0;
		float maxFound = 0;

		int searchArray[1500];
		FillRandomArray(searchArray, 1500, 0, factor, false, UNSORTED);
		for (int i = 0; i < 1500; i++) {
			float effort = 0;
			int k = hashTableSearch(hashTable, array[searchArray[i]], &effort);
			if (k != 0) {
				if (effort > maxFound) {
					maxFound = effort;
				}

				averageFound = averageFound + effort;
			}
		}

		averageFound = averageFound / 1500;

		medFound = medFound + averageFound;

		if (maxFound > medMaxFound) {
			medMaxFound = maxFound;
		}

		float averageNotFound = 0;
		float maxNotFound = 0;

		int biggerValues[1500];
		FillRandomArray(biggerValues, 1500, 30000, 50000, false, UNSORTED);
		for (int i = 0; i < 1500; i++) {
			float effort = 0;
			int k = hashTableSearch(hashTable, biggerValues[i], &effort);

			if (k == 0) {
				if (effort > maxNotFound) {
					maxNotFound = effort;
				}

				averageNotFound = averageNotFound + effort;
			}
		}

		averageNotFound = averageNotFound / 1500;

		medNotFound = medNotFound + averageNotFound;

		if (maxNotFound > medMaxNotFound) {
			medMaxNotFound = maxNotFound;
		}
	}
	medFound = medFound / 5;
	medNotFound = medNotFound / 5;
	printf("Average effort found %.2f\n", medFound);
	printf("Max effort found %.2f\n", medMaxFound);

	printf("Average effort not found : %.2f \n", medNotFound);
	printf("Max effort not found : %.2f ", medMaxNotFound);

	intHashTableFree(hashTable);

}

void teste() {
	printf("Pentru factor de umplere 0,80 : \n");
	testEffort(7978);
	printf("\n");
	printf("\nPentru factor de umplere 0,85 : \n");
	testEffort(8477);
	printf("\n");
	printf("\nPentru factor de umplere 0,90 : \n");
	testEffort(8975);
	printf("\n");
	printf("\nPentru factor de umplere 0,95 : \n");
	testEffort(9474);
	printf("\n");
	printf("\nPentru factor de umplere 0,99 : \n");
	testEffort(9873);
}

void deletePositionFromArray(int vector[], int *vectorSize, int position) {
	for (int i = position; i < *vectorSize; i++) {
		vector[i] = vector[i + 1];
	}
	(*vectorSize)--;
}

void testeEffortCuDelete() {
	int n = 9973;
	IntHashTable* hashTable = intHashTableNew(n);

	int size = 9973;
	int array[9973];
	FillRandomArray(array, 9873, 10, 25000, false, UNSORTED);
	for (int i = 0; i < size; i++) {
		intHashTableInsert(hashTable, array[i], array[i]);
	}
	
	int deleteArray[1895];
	FillRandomArray(deleteArray, 1895, 0, 9872, false, UNSORTED);

	for (int i = 0; i < 1895; i++) {
		intHashTableDelete(hashTable, array[deleteArray[i]]);
		deletePositionFromArray(array, &size, deleteArray[i]);
	}

	testEffort(size);

	intHashTableFree(hashTable);
}

int main() {
	//demo();
	//demoBiggerValues();
	//teste();
	testeEffortCuDelete();
	return 0;
}