/*Nilvan Roxana Maria 
* grupa 30228
* In aceasta tema am implementat doi algoritmi de transformare a arborilor. Prima transformare din reprezentare parinte in reprezentare de tip arbore multicai,
* iar a doua transformare din reprezentare arbore multicai, in reprezentare de tip arbore binar.
* 
* Problema impusa de prima transformare este urmatoarea : tranfrmarea arborelui din reprezentare parinte in reprezentare de tip arbore multicai in complexitate O(n)
* Pentru a respecta cerinta impusa avem nevoie de O(n) memorie aditionala. Fara a folosi memorie aditionala algoritmul are complexitate O(n^2)
* Pasii de rezolvare sunt urmatorii :
* - am alocat memorie pentru radacina propriu zisa si pentru un array de noduri de tip radacina
* - am parcurs prima data sirul si am cautat radacina (elementul care are valoare "-1") atribuind-ul atat pozitei din array-ul de noduri cat si radacini. In acest
* moment, atat radacina cat si elementu din array-ul de noduri pointeaza spre acceasi valoare de memorie.
* - am parcurs inca o data sirul si am alocat memorie pozitiilor nealocate deja 
* - am parcurs pentru a treia oara sirul si am ralizat legaturile in modul urmator : nodul copil este legat la lista de copii al nodului parinte si numarul de copii
* ai nodului parinte creste
* 
* Consider ca algoritmul este eficent din punct de vedere al timpului de rulare deoarece respecta complexitatea ceruta. Chiar daca nu este cel mai eficient din 
* punct de vedere al memorie, foloseste O(n) memorie aditionala, aceasta memorie poate fi eliberata.
* 
* Pentru a realiza a doua transformarea am aplicat urmatoarea strategie :
* - am alocat memorie pentru arborele radacina si am introdus valoarea chei
* - am verificat daca arborele are copii, iar daca are am legat in partea stranga primul copil, reapeland algoritmul recursiv
* - mai apoi am parcurs lista de copii al nodului, legandu-i in partea dreapta a nodului stranga creat , reapelang algoritmul recursiv
* 
* Prety print-ul arborelui binar afiseaza rezultatul in felul urmator :
*			copilul din dreapta
* cheia
*			copilul din stanga
* 
* In reprezentarea noasta : parintele 7, copil stanga 2, dreapta lui 2 -> 5, dreapta lui 5 -> 6, stranga lui 5 -> 3, dreapta lui 3 -> 8
*							stanga lui 2 -> 1, dreapta lui 1 -> 4 si dreapta lui 4 -> 9
*/
#include<stdio.h>
#include <stdlib.h>
#define MAX_VALUE 100

typedef struct multiwayTree {
	int key;
	int numberOfKids;
	int procesed;
	struct multiwayTree *arrayOfMultiwayKids[MAX_VALUE];
}multiway;

typedef struct IntBinaryTree {
	int data;
	unsigned size;
	IntBinaryTree *left; // first kid
	IntBinaryTree *right; // first sibling
}IntBinaryTree;

multiwayTree* multiwayTreeNew() {
	multiwayTree* tree =(multiwayTree*) malloc(sizeof(multiwayTree));
	tree->key = NULL;
	tree->numberOfKids = 0;
	tree->procesed = 0;
	
	return tree;
}

multiwayTree* multiwayTreeData(int value) {
	multiwayTree* tree = (multiwayTree*)malloc(sizeof(multiwayTree));
	tree->key = value;
	tree->numberOfKids = 0;
	tree->procesed = 0;

	return tree;
}

IntBinaryTree* intBinaryTreeNew() {
	IntBinaryTree* tree = (IntBinaryTree*)malloc(sizeof(IntBinaryTree));
	tree->size = 0;
	tree->data = 0;
	tree->left = NULL;
	tree->right = NULL;
	return tree;
}

void findKids(int parent,int arrayOfPerents[], int size, int arrayOfKinds[], int *numberOfKids) {
	for (int i = 0; i < size; i++) {
		if (parent == arrayOfPerents[i]) {
			arrayOfKinds[*numberOfKids] = i + 1;
			(*numberOfKids)++;
		}
	}

}

void printSpace(int depth) {
	for (int i = 0; i < depth; i++) {
		printf("   ");
	}
}

void pretyPrintParentReprezentation(int arrayOfPerents[], int size, int root, int depth) {
	int numberOfKids = 0;
	int arrayOfKids[MAX_VALUE] = {};

	findKids(root, arrayOfPerents, size, arrayOfKids, &numberOfKids);

	for (int i = 0; i < numberOfKids; i++) {
		printSpace(depth);
		printf("%d\n", arrayOfKids[i]);
		pretyPrintParentReprezentation(arrayOfPerents, size, arrayOfKids[i], depth + 1);
	}
}

multiwayTree *firstTransformation(int arrayOfPerents[], int size) {
	multiwayTree *root = multiwayTreeNew();
	multiwayTree **arrayOfnodes = (multiwayTree**)calloc(size, sizeof(multiwayTree));


	for (int i = 0; i < size; i++) {
		if (arrayOfPerents[i] == -1) {
			if (arrayOfnodes[i] == NULL) {
				arrayOfnodes[i] = multiwayTreeData(i + 1);
				root = arrayOfnodes[i];
			}
			else {
				root = arrayOfnodes[i];
			}
		}
	}
	for (int i = 0; i < size; i++) {
		if (arrayOfnodes[i] == NULL) {
			arrayOfnodes[i] = multiwayTreeData(i + 1);
		}
	}
	for (int i = 0; i < size; i++) {
		if (arrayOfPerents[i] != -1) {
			int number = arrayOfnodes[arrayOfPerents[i] - 1]->numberOfKids;

			arrayOfnodes[arrayOfPerents[i] - 1]->arrayOfMultiwayKids[number] = arrayOfnodes[i];
			arrayOfnodes[arrayOfPerents[i] - 1]->numberOfKids++;
		}
	
	}

	return root;
}


void pretyPrintMultiwayReprezentation(multiway* root, int depth) {
	printSpace(depth);
	printf("%d\n", root->key);
	for (int i = 0; i < root->numberOfKids; i++) {
			pretyPrintMultiwayReprezentation(root->arrayOfMultiwayKids[i], depth + 1);
	}
}

IntBinaryTree* intBinaryTreeNewInt(int value) {
	IntBinaryTree* newTree = (IntBinaryTree*)malloc(sizeof(IntBinaryTree));
	newTree->data = value;
	newTree->left = NULL;
	newTree->right = NULL;
	newTree->size = 1;

	return newTree;
}

IntBinaryTree *secondTransformation(multiway *root) {
	IntBinaryTree* tree = intBinaryTreeNewInt(root->key);
	
	if (root->numberOfKids > 0) {
		tree->left = secondTransformation(root->arrayOfMultiwayKids[0]);
	}

	IntBinaryTree* rightBranchOfRightNode = tree->left;
	for (int i = 1; i < root->numberOfKids; i++) {
		rightBranchOfRightNode->right = secondTransformation(root->arrayOfMultiwayKids[i]);
		rightBranchOfRightNode = rightBranchOfRightNode->right;
	}

	return tree;
}

void pretyPrint(IntBinaryTree* tree, int space) {
	if (tree == NULL) {
		return;
	}

	space = space + 4;

	pretyPrint(tree->right, space);

	printf("\n");
	for (int i = 4; i < space; i++) {
		printf(" ");
	}

	printf("%d\n", tree->data);

	pretyPrint(tree->left, space);
}

void intBinaryTreePreorder(IntBinaryTree* tree) {
	if (tree != NULL) {
		printf("%d ", tree->data);
		intBinaryTreePreorder(tree->left);
		intBinaryTreePreorder(tree->right);
	}
}

void demo() {
	int array[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
	int size = sizeof(array) / sizeof(array[0]);
	printf("Pretty print parent reprezentation : \n");
	pretyPrintParentReprezentation(array, size, -1, 0);

	multiwayTree* root = multiwayTreeNew();
	root = firstTransformation(array, size);
	printf("\n");
	printf("Pretty print multiway tree reprezentation : \n");
	pretyPrintMultiwayReprezentation(root, 0);

	printf("\n");
	IntBinaryTree* tree = intBinaryTreeNew();
	tree = secondTransformation(root);
	printf("Pretty print binary tree reprezentation : \n");
	pretyPrint(tree, 0);
}

int main() {
	demo();
	return 0;
}