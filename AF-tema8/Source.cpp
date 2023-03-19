/*Nilvan Roxana Maria
* Grupa 30228
* In aceasta tema am implementat doua metode de afisare a unui arbore binar atat in mod recuriiv cat si in mod iterativ. Si o metoda de hibiridizare a quick
* sort-ului.
* 
* Pentu afiseara recursiva a arborelui am apicat urmatoarea strategie : am procesat cheia si am apelat algoritmul pe partea stanga si dreapta a arborelui.
* Pentru afiseara iterativa am aplicat urmatoarea strategie : daca subarborele stanga este null afisam cheia si algoritmul continua pe partea dreapta. Daca nu
* salvam intr-o variabila subarborele stanga si ii parcurgem subarborele drept pana cand ajungem la frunza. Daca subarborele drep este egal cu arborele ii atribuim
* valoarea null si continuam parcugerea pe partea stanga. Daca nu afisam cheia, in subarborele drep mutam radacina si in radacina mutam copilul stanga.
* 
* Am realizat si un grafic al afisarilor pentru cei doi algoritmi si am observat ca ata varianta de preorder iterativ cat si cea de preorder recursiv realizeaza 
* accelasi numar de afisari. Din punct de vedere al operatiilor varianta recursiva realizeaza doar o comparatie la fiecare aple, iar cea iterativa realizeaza
* doua comparatii si doua atribuiri.
* 
* Hibridizarea quick sotului am procedat in modul urmator : am testat daca indicele stanga este mai mic decat indicele dreapta, daca da, daca indicele stanga
* + indicele dreapta + 1 sunt mai mici decat valoarea rezultata este mai mica decat valoarea de "cut" apelam algoritmul de sortare prin insertie , daca nu 
* aplicam algoritmul de hibridizare pe partea stanga a sirului si pe partea dreapta a sirului.
* 
* Din masuratorile realizate putem observa ca din punct de vedere al operatiilor quick sort-ul normal este mai rapid decat quick sortul normal. Din masuratorile
* realizate pentru timpi de executare observam ca tot quick sort-ul normal este putin mai rapid.
*/
#include<stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("tema8");

typedef struct IntBinaryTree {
    int size;
    int data;
    IntBinaryTree* left;
    IntBinaryTree* right;
}IntBinaryTree;

IntBinaryTree* intBinaryTreeNew() {
    IntBinaryTree* tree = (IntBinaryTree*)malloc(sizeof(IntBinaryTree));
    tree->size = 0;
    tree->data = 0;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}

void intBinaryTreeFree(IntBinaryTree* tree) {
    if (tree) {
        IntBinaryTree* tree_free = tree;
        intBinaryTreeFree(tree->left);
        intBinaryTreeFree(tree->right);
        free(tree_free);
    }
    free(tree);
}

IntBinaryTree* intBinaryTreeNewValue(int value) {
    IntBinaryTree* new_tree = (IntBinaryTree*)malloc(sizeof(IntBinaryTree));
    new_tree->data = value;
    new_tree->size = 1;
    new_tree->left = NULL;
    new_tree->right = NULL;
    return new_tree;

}
void intBinaryTreeAdd(IntBinaryTree* tree, int value) {
    if (tree) {
        if (tree->size == 0) {
            tree->data = value;
        }
        else if (tree->right == NULL) {
            tree->right = intBinaryTreeNewValue(value);
        }
        else if (tree->left == NULL) {
            tree->left = intBinaryTreeNewValue(value);
        }
        else if (tree->right->size <= tree->left->size) {
            intBinaryTreeAdd(tree->right, value);
        }
        else if (tree->left->size < tree->right->size) {
            intBinaryTreeAdd(tree->left, value);
        }
        tree->size = tree->size + 1;
    }
}

void intBinaryTreePreorder(IntBinaryTree* tree, Operation *recursive) {
    if (tree) {
        recursive->count();
        //printf("%d ", tree->data);
        intBinaryTreePreorder(tree->left, recursive);
        intBinaryTreePreorder(tree->right, recursive);
    }
}

void intBinaryTreePreorderIterativ(IntBinaryTree* tree, Operation *iterative) {
    while (tree) {
        if (tree->left == NULL) {
            iterative->count();
            //printf("%d ", tree->data);
            tree = tree->right;
        }
        else {
            IntBinaryTree* currentTree = tree->left;
            while (currentTree->right && currentTree->right != tree) {
                currentTree = currentTree->right;
            }

            if (currentTree->right == tree) {
                currentTree->right = NULL;
                tree = tree->right;
            }
            else {
                iterative->count();
                //printf("%d ", tree->data);
                currentTree->right = tree;
                tree = tree->left;
            }
        }
   } 

}

void intBinaryTreePreorderDemo(IntBinaryTree* tree) {
    if (tree) {
        printf("%d ", tree->data);
        intBinaryTreePreorderDemo(tree->left);
        intBinaryTreePreorderDemo(tree->right);
    }
}

void intBinaryTreePreorderIterativDemo(IntBinaryTree* tree) {
    while (tree) {
        if (tree->left == NULL) {
            printf("%d ", tree->data);
            tree = tree->right;
        }
        else {
            IntBinaryTree* currentTree = tree->left;
            while (currentTree->right && currentTree->right != tree) {
                currentTree = currentTree->right;
            }

            if (currentTree->right == tree) {
                currentTree->right = NULL;
                tree = tree->right;
            }
            else {
                printf("%d ", tree->data);
                currentTree->right = tree;
                tree = tree->left;
            }
        }
    }

}

void pretyPrint(IntBinaryTree* tree, int space) {
    if (tree == NULL) {
        return;
    }

    space = space + 5;

    pretyPrint(tree->right, space);

    printf("\n");
    for (int i = 5; i < space; i++) {
        printf(" ");
    }

    printf("( %d ) \n", tree->data);

    pretyPrint(tree->left, space);
}

// Quicksort hybridization

void insertionSort(int array[], int leftIndex, int rightIndex, Operation* hybridQuickSortOperations)
{

    for (int i = leftIndex + 1; i < rightIndex + 1; i++){
        hybridQuickSortOperations->count();
        int key = array[i];
        int j = i;
        hybridQuickSortOperations->count();
        while (j > leftIndex && array[j - 1] > key){
            hybridQuickSortOperations->count(2);
            array[j] = array[j - 1];
            j -= 1;
        }
        hybridQuickSortOperations->count();
        array[j] = key;
    }

}

int partition(int array[], int leftIndex, int rightIndex, Operation* hybridQuickSortOperations)
{
    hybridQuickSortOperations->count();
    int pivot = array[rightIndex];
    int i, j;
    i = leftIndex;
    j = leftIndex;

    for (int i = leftIndex; i < rightIndex; i++)
    {
        hybridQuickSortOperations->count();
        if (array[i] < pivot)
        {
            hybridQuickSortOperations->count(3);
            int aux = array[i];
            array[i] = array[j];
            array[j] = aux;
            j += 1;
        }
    }

    hybridQuickSortOperations->count(3);
    int aux = array[j];
    array[j] = array[rightIndex];
    array[rightIndex] = aux;

    return j;
}

void quickSort(int array[], int leftIndex, int rightIndex, Operation* hybridQuickSortOperations)
{
    if (leftIndex < rightIndex) {
        int indexOfPartition = partition(array, leftIndex, rightIndex, hybridQuickSortOperations);
        quickSort(array, leftIndex, indexOfPartition - 1, hybridQuickSortOperations);
        quickSort(array, indexOfPartition + 1, rightIndex, hybridQuickSortOperations);
    }
}

void hybridQuickSort(int array[], int leftIndex, int rightIndex,int cutoff, Operation *hybridQuickSortOperations) {
    while (leftIndex < rightIndex) {
        if (rightIndex - leftIndex + 1 < cutoff) {
            insertionSort(array, leftIndex, rightIndex, hybridQuickSortOperations);
            return;
        }
        else {
            int pivot = partition(array, leftIndex, rightIndex, hybridQuickSortOperations);
            if (pivot - leftIndex < rightIndex - pivot) {
                hybridQuickSort(array, leftIndex, pivot - 1, cutoff, hybridQuickSortOperations);
                leftIndex = pivot + 1;
            }
            else {
                hybridQuickSort(array, pivot + 1, rightIndex, cutoff, hybridQuickSortOperations);
                rightIndex = pivot - 1;
            }
        }
    }
}

void demo() {
    printf("The tree operations : \n");
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = 10;

    IntBinaryTree* tree = intBinaryTreeNew();

    for (int i = 0; i < size; i++) {
        intBinaryTreeAdd(tree, array[i]);
    }
    pretyPrint(tree, 0);

    printf("The recursive method : \n");
    intBinaryTreePreorderDemo(tree);
    printf("\n");
    printf("The iterative method : \n");
    intBinaryTreePreorderIterativDemo(tree);

    printf("\n");
    printf("\n");
    printf("Quicksort hybridization : \n");

    int cutoff = 30;

    printf("Array smaller than 30 : \n");
    int array2[100];
    int size2 = 15;
    FillRandomArray(array2, size2, 1, 500, false, UNSORTED);
    Operation hybrridOperations2 = p.createOperation("hybridQuickSortOperations2", size2);
    printf("Before sort \n");
    for (int i = 0; i < size2; i++) {
        printf("%d ", array2[i]);
    }
    printf("\n");
    hybridQuickSort(array2, 0, size2 - 1, cutoff, &hybrridOperations2);
    printf("\nAfter sort \n");
    for (int i = 0; i < size2; i++) {
        printf("%d ", array2[i]);
    }
    printf("\n");
    printf("\n");
    printf("\nArray with size bigger than 30 :\n");
    int array3[100];
    int size3 = 100;
    FillRandomArray(array3, size3, 1, 500, false, UNSORTED);
    Operation hybrridOperations3 = p.createOperation("hybridQuickSortOperations3", size3);
    printf("Before sort \n");
    for (int i = 0; i < size3; i++) {
        printf("%d ", array3[i]);
    }
    printf("\n");
    hybridQuickSort(array3, 0, size3 - 1, cutoff, &hybrridOperations3);
    printf("\nAfter sort \n");
    for (int i = 0; i < size3; i++) {
        printf("%d ", array3[i]);
    }

}

void teste() {
    int array[MAX_SIZE];
    int arraySize;
    for (arraySize = STEP_SIZE; arraySize <= MAX_SIZE; arraySize += STEP_SIZE) {
        for (int test = 0; test < NR_TESTS; test++) {
            Operation recursive = p.createOperation("recursive", arraySize);
            Operation iterative = p.createOperation("iterative", arraySize);

            FillRandomArray(array, arraySize, 10, 50000, false, UNSORTED);
            IntBinaryTree* tree = intBinaryTreeNew();
            for (int i = 0; i < arraySize; i++) {
                intBinaryTreeAdd(tree, array[i]);
            }

            intBinaryTreePreorder(tree, &recursive);
            intBinaryTreePreorderIterativ(tree, &iterative);
    }   
}
    p.divideValues("recursive", NR_TESTS);
    p.divideValues("iterative", NR_TESTS);

    p.createGroup("preorderAnaliza", "recursive", "iterative");

    p.showReport();
}

void testHybridQuikSort() {
    int cutoff = 100;
    int array[MAX_SIZE], array2[MAX_SIZE];
    int arraySize;
    for (arraySize = STEP_SIZE; arraySize <= MAX_SIZE; arraySize += STEP_SIZE) {
        for (int test = 0; test < NR_TESTS; test++) {
            Operation hybrridOperations = p.createOperation("hybridQuickSortOperations", arraySize);
            Operation quickSortOperations = p.createOperation("QuickSortOperation", arraySize);

            FillRandomArray(array, arraySize, 10, 50000, false, UNSORTED);
            CopyArray(array2, array, arraySize);

            hybridQuickSort(array, 0, arraySize - 1, cutoff, &hybrridOperations);
            quickSort(array2, 0, arraySize - 1, &quickSortOperations);
        }
    }
    p.divideValues("hybridQuickSortOperations", NR_TESTS);
    p.divideValues("QuickSortOperation", NR_TESTS);

    p.createGroup("SortOperation", "hybridQuickSortOperations", "QuickSortOperation");

    p.showReport();
}

void time() {
    p.reset("quickSort-vs-HybridSort-time");
    const int step = 100;
    const int time_test = 100;
    const int max_size2 = step * 100;
    int cutoff = 100;

    int vector1[max_size2], vector2[max_size2], vector3[max_size2], vector4[max_size2];

    for (int n = step; n < max_size2; n += step) {
        FillRandomArray(vector1, n);
        CopyArray(vector2, vector1, n);
        Operation hybrridOperations1 = p.createOperation("hybridQuickSortOperations1", n);
        Operation quickSortOperations1 = p.createOperation("QuickSortOperation1", n);

        hybridQuickSort(vector1, 0, n - 1, cutoff, &hybrridOperations1);
        quickSort(vector2, 0, n - 1, &quickSortOperations1);
    }

    for (int n = step; n <= max_size2; n += step) {
        FillRandomArray(vector3, n);
        CopyArray(vector4, vector3, n);

        Operation hybrridOperations1 = p.createOperation("hybridQuickSortOperations1", n);

        p.startTimer("hybridQuickSortOperations1", n);
        for (int test = 0; test < time_test; test++) {
            hybridQuickSort(vector3, 0, n - 1, cutoff, &hybrridOperations1);
        }
        p.stopTimer("hybridQuickSortOperations1", n);

        Operation quickSortOperations1 = p.createOperation("QuickSortOperation1", n);

        p.startTimer("QuickSortOperation1", n);
        for (int test = 0; test < time_test; test++) {
            quickSort(vector4, 0, n - 1, &quickSortOperations1);
        }
        p.stopTimer("QuickSortOperation1", n);
    }

    p.createGroup("time", "hybridQuickSortOperations1", "QuickSortOperation1");
    p.showReport();
}

void testPrag() {
    int arraySize = MAX_SIZE;
    int array[MAX_SIZE], array2[MAX_SIZE];
    FillRandomArray(array, arraySize, 10, 50000, false, UNSORTED);
    for (int cutoff = 1; cutoff <= 50; cutoff += 5) {
        for (int test = 0; test < NR_TESTS; test++) {
            CopyArray(array2, array, MAX_SIZE);
            Operation pragOP = p.createOperation("prag", cutoff);
            hybridQuickSort(array2, 0, arraySize - 1, cutoff, &pragOP);
        }
    }
    p.divideValues("prag", NR_TESTS);
    p.showReport();
}

int main() {
    demo();
    //teste();
    //time();
    //testHybridQuikSort();
   //testPrag();
	return 0;
}