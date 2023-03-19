/*Nilvan Roxana Maria
* Grupa 30228
* In aceasta tema am implementat operatii pe multimi disjuncte si algoritmul lui Kruskal ajutandu-ne de aceste multimi.
* Functiile implementate prntru multimi disjuncte sunt urmatoarele :
* Make_Set : creaza o noua multime a carui singur reprezentant este nodul in sine
* Find_Set : cauta si retunrneaza reprezentantul unei multimi
* Union : uneste doua multimi dupa urmatoarea strategie : cauta reprezentantul fiecarei multimi si compara rangul lor, daca rangul
*		  primei multimi este mai mare, uneste cea de a doua multime la prima multime, in caz de egalitate, multimea unu este legata
*		  la multimea doi, iar rangul multimii doi creste
* 
* Algoritmul lui Kruskal aplica urmatoarea strategie :
* - primeste ca parametru un numar de noduri, un vector de costuri si un vector de muchii in care se va stoca arborele de acoperire cu
* costul cel mai mic si returneaza numarul de muchii intorduse in arbore
* - se initializeaza nodurile si pentru fiecare nod se creaza cate o multime
* - se pargur muchiile si se creaza cate o legatura intre nodurile adiacente pentru a ne asigura ca graful este conex
* - generam doua siruri de noduri aleatoare de lungime 3 * n - 1 pe care le unim
* - la fiecare muchie ii este atribuit un cost generat aleator
* - sortam muchiile in ordine crescatoare a costului
* - parcurgem muchiile in felul urmator : daca nodul "parinte" si nodul "copil" nu au acelasi reprezentant al multimii, muchia este
*										  adaugata in arbore, si plicam operatia de union pentru cele doua noduri.
* 
* Din masuratorile realizate observam ca operatiile pe multimile disjuncte in algoritmul lui Kruskal au complexitatea de O(n).
*/

#include<stdio.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5


Profiler p("Disjoint_Sets_and_Kruskal");

typedef struct Node {
	int key;
	int rank;
	Node* parent;
}Node;

typedef struct Edge {
	int weight;
	Node* head;
	Node* tail;
}Edge;

Node* intNewNodeData(int key) {
	Node* newNode = (Node*)calloc(sizeof(Node), 1);
	if (newNode != NULL) {
		newNode->key = key;
		newNode->rank = 0;
		newNode->parent = NULL;
	}
	return newNode;
}


Edge* intNewEdgeData(Node* head, Node* tail, int weight) {
	Edge* newEdge = (Edge*)malloc(sizeof(Edge));
	if (newEdge != NULL) {
		newEdge->weight = weight;
		newEdge->tail = tail;
		newEdge->head = head;
	}
	return newEdge;
}

void MAKE_SET(Node* toSet, Operation* op) {
	toSet->parent = toSet;
	toSet->rank = 0;
	op->count(2);
}

Node* FIND_SET(Node* toFind, Operation* op) {
	op->count();
	if (toFind != toFind->parent) {
		toFind->parent = FIND_SET(toFind->parent, op);
		op->count();
	}
	return toFind->parent;
}

void Link(Node* node1, Node* node2, Operation* op) {
	op->count();
	if (node1->rank > node2->rank) {
		node2->parent = node1;
		op->count();
	}
	else {
		op->count();
		node1->parent = node2;
		op->count();
		if (node1->rank == node2->rank) {
			node2->rank++;
			op->count();
		}
	}
}

void Union(Node* node1, Node* node2, Operation* op) {
	Link(FIND_SET(node1, op), FIND_SET(node2, op), op);
}

void sortEdges(Edge* array[], int numberOfEdges) {
	int minPoz;
	for (int i = 0; i < numberOfEdges - 1; i++) {
		minPoz = i;
		for (int j = i + 1; j < numberOfEdges; j++) {
			if (array[j]->weight < array[minPoz]->weight) {
				minPoz = j;
			}
		}
		if (i != minPoz) {
			Edge* aux = array[i];
			array[i] = array[minPoz];
			array[minPoz] = aux;
		}
	}
}

int Kruskal(int numberOfNodes, int arrayOfWeight[], Edge* mstKruskal[], Operation* op) {
	Node* arrayOfNodes[MAX_SIZE];
	for (int i = 0; i < numberOfNodes; i++) {
		arrayOfNodes[i] = intNewNodeData(i + 1);
		MAKE_SET(arrayOfNodes[i], op);
	}

	Edge* arrayOfEdges[4 * MAX_SIZE];

	int currentPoz = 0;
	int currentEdge = 0;
	for (int i = 0; i < numberOfNodes - 1; i++) {
		arrayOfEdges[currentEdge] = intNewEdgeData(arrayOfNodes[i], arrayOfNodes[i + 1], arrayOfWeight[currentPoz]);
		currentPoz++;
		currentEdge++;
	}

	if (numberOfNodes > 0) {
		arrayOfEdges[currentEdge] = intNewEdgeData(arrayOfNodes[0], arrayOfNodes[numberOfNodes - 1], arrayOfWeight[currentPoz]);
		currentPoz++;
		currentEdge++;
	}

	int needed = 3 * numberOfNodes - 1;
	int head[4 * MAX_SIZE];
	FillRandomArray(head, needed, 1, numberOfNodes - 1, false, UNSORTED);

	int tail[4 * MAX_SIZE];
	FillRandomArray(tail, needed, 1, numberOfNodes - 1, false, UNSORTED);

	for (int i = 0; i < needed; i++) {
		if (head[i] != tail[i]) {
			arrayOfEdges[currentEdge] = intNewEdgeData(arrayOfNodes[head[i]], arrayOfNodes[tail[i]], arrayOfWeight[currentPoz]);
			currentPoz++;
			currentEdge++;
		}
	}

	/*for (int i = 0; i < currentEdge; i++) {
		printf("%d -- %d -- %d \n", arrayOfEdges[i]->head->key, arrayOfEdges[i]->tail->key, arrayOfEdges[i]->weight);
	}
	*/

	int currentKruskal = 0;
	sortEdges(arrayOfEdges, currentEdge);

	for (int i = 0; i < currentEdge; i++) {
		Node* parent = arrayOfEdges[i]->head;
		Node* kid = arrayOfEdges[i]->tail;

		if (FIND_SET(parent, op) != FIND_SET(kid, op)) {
			mstKruskal[currentKruskal] = arrayOfEdges[i];
			currentKruskal++;
			Union(parent, kid, op);
		}
	}
	return currentKruskal;
}

void demo() {
	printf("Demo multimi disjuncte : \n ");
	printf("\n");
	int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int size = sizeof(array) / sizeof(array[0]);
	Operation kruskal1 = p.createOperation("KruskalAlgorithm1", size);
	int cost[10];
	FillRandomArray(cost, 10, 1, 10, false, UNSORTED);
	Node* set[10]{};

	for (int i = 0; i < size; i++) {
		set[i] = intNewNodeData(array[i]);
		MAKE_SET(set[i], &kruskal1);
	}

	for (int i = 0; i < size - 3; i++) {
		Union(set[i], set[i + 3], &kruskal1);

	}

	for (int i = 0; i < size; i++) {
		printf("Node: %d, Parent: %d\n", set[i]->key, set[i]->parent->key);
	}

	printf("\n");

	for (int i = 0; i < size / 2; i++) {
		Node* intermediate = FIND_SET(set[i + 3], &kruskal1);
		printf("Node found : %d, at search element : %d \n", intermediate->key, i + 3);
	}
	printf("\n");
	printf("\n");

	printf("Demo Kruskal : \n");

	int weight[70];
	FillRandomArray(weight, 70, 1, 10, false, UNSORTED);

	Edge* mstKruskal[MAX_SIZE]{};

	int sizeKruskal = 0;
	printf("\n");
	sizeKruskal = Kruskal(15, weight, mstKruskal, &kruskal1);
	printf("\n");

	for (int i = 0; i < sizeKruskal; i++) {
		if (mstKruskal[i] != NULL) {
			printf("%d -- %d -- %d\n", mstKruskal[i]->head->key, mstKruskal[i]->weight, mstKruskal[i]->tail->key);
		}
	}
}
void test() {
	int arraySize;
	for (arraySize = 100; arraySize < 10000; arraySize += 100) {
		for (int test = 0; test < NR_TESTS; test++) {

			Operation kruskal = p.createOperation("KruskalAlgorithm", arraySize);

			int weight[4 * MAX_SIZE];
			int numberOfEdges = 4 * arraySize;
			FillRandomArray(weight, numberOfEdges, 1, 50000, false, UNSORTED);
			Edge* mstKruskal[MAX_SIZE];
			int size = Kruskal(arraySize, weight, mstKruskal, &kruskal);
			//freeEdges(mstKruskal, size);
		}
	}
	p.divideValues("KruskalAlgorithm", NR_TESTS);
	p.showReport();
}
int main() {
	demo();
	//test();
	return 0;
}