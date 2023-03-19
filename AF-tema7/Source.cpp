/* Nilvan Roxana Maria 
*  Grupa 30228
* In aceasta tema am implementat statistici dinamice de ordine pe arbori binari de cautare si am evaluat operatiile de construire, selectare si stergere intr-un
* astfel de arbore. Algoritmul se imparte in trei parti :
*
I Partea de construire a arborelui :
* - este construita din doua functii : o functie care primeste un arbore gol, un vector sortat crescator, lungimea si pozitia de start, iar cea de a doua functie
* primeste un arbore si o valoare ce va fi introdusa in arbore
* - prima functie functioneaza in felul urmator :
*	- vefifica daca nu am ajuns cu pozitia de start la pozitia finala, iar daca nu gaseste elementul din mijloc si il introduce in arbore
*	- se apeleaza recursiv aceasta functie ata de partea dreapta cat si pe partea dreapta a sirului cat si pe partea stanga a sirului
* - cea de a doua functie functioneaza in felul urmator :
*	- primeste un arbore si valoarea pe care trebuie sa o introduca in arbore
*	- verifica daca arborele este gol, daca este col valoarea primita este introdusa ca radacina
*	- daca avem valori introduse in arbore verifica daca valoarea introdusa este mai mare decat radacina, daca da si subarborele din deapta este gol, valoarea este
*	adaugata ca si copil dreapta, daca subarborele drept nu este gol, se reapleaza algoritmul pentru subarborele cu copilul dreapta ca radacina
*	- daca valoarea introdusa este mai mica decat radacina se executa algoritmul descris mai sus pe subarbirele stang
* Operatia de build are complexitate O(n)
* 
* II Partea de selectare :
* - daca arborele este gol, returneaza NULL
* - daca nu, daca copilul din stanga nu este gol, se salveaza marimea subarborelui stanga + 1, daca copilul stanga este gol se salveaza 1
* - daca marimea subarborelui este marimea cautata se returneaza subarborele
* - daca nu se reapeleaza algoritmul ata pe partea dreapta cat si pe partea stanga 
* Operatia de selectare are compexitate O(n)
* 
* III Partea de stergere :
* - este construita din trei functii o functie care primeste un arbore si o valoare de sters, o functie ajutatoare care sterge valoarea din atbore, si o functie
* care cauta succesorul unui nod
* - prima functie apelaza cea de a doua functie
* - functia de cautare a succesorului parcurge subarborele stang al unui arbore primit pana cand ajunge la final si returneaza ultimul nod
* - functia care sterge parcurge arborele si cauta valoarea trimisa ca si parametru, daca valoarea pe care o cautam e frunza se strge pur si sinplu daca vrem 
* sa stergem un nod parinte apelam functia care caseste succesorul, se pune valoarea returnata de catre succesor in nodul care trebuie sters si se reapeleaza
* algoritmul petru nodul care trebuia sters
* Operatia de stergere are o complexitate O(n)
* 
*/
#include<stdio.h>
#include<stdlib.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("DynamicOrderStatistics");

typedef struct IntBinarySearchTree {
	int data;
	unsigned defined;
	int size;
	IntBinarySearchTree* left;
	IntBinarySearchTree* right;
}IntBinarySearchTree;

IntBinarySearchTree* intBinarySearchTreeNew() {
	IntBinarySearchTree* tree = (IntBinarySearchTree*)malloc(sizeof(IntBinarySearchTree));
	tree->data = 0;
	tree->defined = 0;
	tree->size = 0;
	tree->left = NULL;
	tree->right = NULL;
	return tree;
}

void intBinarySearchTreeFree(IntBinarySearchTree* tree) {
	if (tree != NULL) {
		intBinarySearchTreeFree(tree->left);
		intBinarySearchTreeFree(tree->right);
		free(tree);
	}
}

IntBinarySearchTree* intBinarySearchTreeNewInt(int value) {
	IntBinarySearchTree* newTree = (IntBinarySearchTree*)malloc(sizeof(IntBinarySearchTree));
	newTree->data = value;
	newTree->defined = 1;
	newTree->size = 1;
	newTree->left = NULL;
	newTree->right = NULL;
	return newTree;
}

unsigned intBinarySearchTreeSize(IntBinarySearchTree* tree) {
	if (tree != NULL)
		return 1 + intBinarySearchTreeSize(tree->right) + intBinarySearchTreeSize(tree->left);
	else return 0;
}

void intBinarySearchTreeAdd(IntBinarySearchTree* tree, int value, Operation *osBuild) {
	osBuild->count();
	if (tree != NULL) {
		if (tree->defined == 0) {
			tree->data = value;
			tree->defined = 1;
			tree->size = intBinarySearchTreeSize(tree);
			return;
		}
		osBuild->count();
		if (tree->data == value) {
			return;
		}
		osBuild->count();
		if (value > tree->data) {
			osBuild->count();
			if (tree->right == NULL) {
				osBuild->count();
				tree->right = intBinarySearchTreeNewInt(value);
				tree->size = intBinarySearchTreeSize(tree);
			}
			else {
				intBinarySearchTreeAdd(tree->right, value, osBuild);
			}
			return;
		}
		osBuild->count();
		if (value < tree->data) {
			osBuild->count();
			if (tree->left == NULL) {
				osBuild->count();
				tree->left = intBinarySearchTreeNewInt(value);
				tree->size = intBinarySearchTreeSize(tree);
			}
			else {
				intBinarySearchTreeAdd(tree->left, value, osBuild);
			}
			return;
		}
	}
}

void buildTree(IntBinarySearchTree* tree, int array[], int left, int right, Operation *osBuild) {
	if (left == right) {
		return;
	}

	int middle = (left + right) / 2;
	intBinarySearchTreeAdd(tree, array[middle], osBuild);

	buildTree(tree, array, left, middle, osBuild);
	buildTree(tree, array, middle + 1, right, osBuild);

	tree->size = intBinarySearchTreeSize(tree);
}

IntBinarySearchTree* OS_Select(IntBinarySearchTree* tree, int searchedIndex, Operation *osSelect) {
	osSelect->count();
	if (tree == NULL) {
		return NULL;
	}
	else {
		int value;
		osSelect->count();
		if (tree->left != NULL) {
			value = tree->left->size + 1;
		}
		else {
			value = 1;
		}

		if (searchedIndex == value) {
			return tree;
		}
		else {
			if (searchedIndex < value) {
				return OS_Select(tree->left, searchedIndex, osSelect);
			}
			else {
				return OS_Select(tree->right, searchedIndex - value, osSelect);
			}
		}
	}
}

int findSuccessor(IntBinarySearchTree* tree, Operation *osDelete) {
	osDelete->count();
	IntBinarySearchTree* current = tree->right;
	osDelete->count();
	while (current->left != NULL) {
		osDelete->count();
		current = current->left;
	}
	return current->data;
}

IntBinarySearchTree* removeHelper(IntBinarySearchTree* tree, int value, Operation *osDelete) {
	osDelete->count();
	if (tree == NULL) return NULL;
	else {
		osDelete->count();
		if (tree->data == value) {
			osDelete->count(2);
			if (tree->right == NULL && tree->left == NULL) {
				free(tree);
				return NULL;
			}
			else {
				osDelete->count(2);
				if (tree->left != NULL && tree->right == NULL) {
					osDelete->count();
					IntBinarySearchTree* temp = tree->left;
					free(tree);
					return temp;
				}
				else {
					osDelete->count(2);
					if (tree->left == NULL && tree->right != NULL) {
						osDelete->count();
						IntBinarySearchTree* temp = tree->right;
						free(tree);
						return temp;
					}
					else {
						int successor = findSuccessor(tree, osDelete);
						osDelete->count(2);
						tree->data = successor;
						tree->right = removeHelper(tree->right, successor, osDelete);
						tree->size--;
						return tree;
					}
				}
			}
		}
		else {
			osDelete->count();
			if (value > tree->data) {
				osDelete->count();
				tree->right = removeHelper(tree->right, value, osDelete);
				tree->size--;
				return tree;
			}
			else {
				osDelete->count();
				tree->left = removeHelper(tree->left, value, osDelete);
				tree->size--;
				return tree;
			}
		}

	}
}

void intBinarySearchTreeRemove(IntBinarySearchTree* tree, int value, Operation *osDelete) {
	removeHelper(tree, value, osDelete);
}
void pretyPrint(IntBinarySearchTree* tree, int space) {
	if (tree == NULL) {
		return;
	}

	space = space + 5;

	pretyPrint(tree->right, space);

	printf("\n");
	for (int i = 5; i < space; i++) {
		printf(" ");
	}

	printf("( %d / %d )\n", tree->data, tree->size);

	pretyPrint(tree->left, space);
}

void demo() {
	int demoSize = 11;
	int array[20];
	IntBinarySearchTree* tree = intBinarySearchTreeNew();
	Operation osBuildDemo = p.createOperation("osSelectDemo", demoSize);
	printf("The Array : ");
	for (int i = 0; i < demoSize; i++) {
		array[i] = i + 1;
		printf(" %d ", array[i]);
	}
	printf("\n");
	printf("The tree :");
	printf("\n");
	buildTree(tree, array, 0, demoSize, &osBuildDemo);
	pretyPrint(tree, 0);

	printf("\n");
	printf("\n");

	IntBinarySearchTree* searchedTree = intBinarySearchTreeNew();
	int searchArray[4] = {};
	FillRandomArray(searchArray, 4, 1, 11, false, UNSORTED);
	for (int i = 0; i < 3; i++) {
		Operation osSelectDemo = p.createOperation("osSelectDemo", demoSize);
		Operation osDeleteDemo = p.createOperation("osDeleteDemo", demoSize);
		printf("%d\n", i);
		searchedTree = OS_Select(tree, searchArray[i], &osSelectDemo);
		pretyPrint(searchedTree, 0);
		printf("\n");

		if (searchedTree != NULL) {
			intBinarySearchTreeRemove(tree, searchedTree->data, &osDeleteDemo);
		}
		printf("The tree after delete :\n ");
		pretyPrint(tree, 0);
		printf("\n");
	}

}

void teste() {
	int array[MAX_SIZE], searchArray[MAX_SIZE];
	int arraySize;
	for (arraySize = STEP_SIZE; arraySize <= MAX_SIZE; arraySize += STEP_SIZE) {
		for (int test = 0; test < NR_TESTS; test++) {
			FillRandomArray(array, arraySize, 1, arraySize, false, ASCENDING);
			FillRandomArray(searchArray, arraySize, 0, arraySize, false, UNSORTED);

			Operation osBuild = p.createOperation("operationBuildTree", arraySize);
			Operation osSelect = p.createOperation("operationSelect", arraySize);
			Operation osDelete = p.createOperation("operationDelete", arraySize);
			
			IntBinarySearchTree* tree = intBinarySearchTreeNew();
			
			buildTree(tree, array, 0, arraySize, &osBuild);

			IntBinarySearchTree* searchedTree = intBinarySearchTreeNew();
			
			for (int i = 0; i < arraySize; i++) {

				searchedTree = OS_Select(tree, searchArray[i], &osSelect);
				if (searchedTree != NULL) {
					intBinarySearchTreeRemove(tree, searchedTree->data, &osDelete);
				}
			}

		}
	}
	p.divideValues("operationBuildTree", NR_TESTS);
	p.divideValues("operationSelect", NR_TESTS);
	p.divideValues("operationDelete", NR_TESTS);
	p.addSeries("DynamicOrderStatistics1", "operationSelect", "operationDelete");
	p.addSeries("DynamicOrderStatistics", "DynamicOrderStatistics1", "operationBuildTree");
	p.showReport();
}

int main() {
	//demo();
	teste();
	return 0;
}