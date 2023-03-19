/*Nilvan Roxana Maria
* grupa 30228
* Am implementat un algoritm pentru interclasarea mai multor liste ordonate.
* Complexitatea algoritmului este de O(nlogk)
* Pentru a demonstra aceasta complexitate am fixat prima data numarul de liste si am variat numarul elementelor totale. Am generat 3 grafice pentru
* 3 numere diferite de liste : 5, 10, 100. Al doilea grafic are numarul de elemente total constant si varieaza numarul de liste.
* 
* Strategia aplicata pentru implementarea algoritmului este urmatoarea:
* - am implementat o functie care genereaza un numar de k liste aleatorii avand in total n elemente. Primele k - 1 liste au n/k elemente, iar cea dea
* " k " lista are restul impartirii lui n la k plus catul impartiri ( int sizeOfLastList = rest + sizeOfEachList; ) pentu a ne asigura ca avem un 
* numar de n elemente in total 
* 
* - in pasul al doilea am implementat o functie care primeste cele k liste generate random si constuieste un vector de noduri din primul element al
* fiecarie liste. Consider ca ideea de a implementa un vector de noduri este mai buna decat indeea de a implementa un vector de liste deoarece memoria 
* ocupata de un nod este mai mica decat memoria ocupata de o lista. Pentru cazurile mici acest avantaj pare mic, dar pentru valori mai se salveaza mai
* multa memorie.
* 
* -in pasul al treilea am implementat functia de merge, in care am urmarit urmatoarele instructiuni. In prima faza am construit un heap cu radacina
* valoarea minima din vectorul de noduri realizat la pasul anterior. Mai apoi am adaugat nodul din radacina pe ultima pozitie a listei ordonate, iar
* mai apoi nudul din radacina a preluat valoarea "next-ului" sau si am reconstruit heap-ul pe minim. In cazul in care am ajuns cu o lista la final, sacde
* marimea heap-ului si in radicina se pune ultima frunza.
* 
* Consider ca algoritmul este eficinet dooarece are capacitatea de a sorta un numar mare de liste cu o compelxitate buna.
*/
#include<stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define MAX_VALUE 10000;
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("kWayMerge");

typedef struct Node {
    int key;
    struct Node* next;
}Node;

typedef struct IntLinkedList {
    int size;
    Node* first;
    Node* last;
}IntLinkedList;

IntLinkedList* intLinkedListNew() {
    IntLinkedList* list = (IntLinkedList*)malloc(sizeof(IntLinkedList));
    list->size = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

void intLinkedListFree(IntLinkedList* list) {
    Node* current;
    while (list->first != NULL) {
        current = list->first;
        list->first = list->first->next;
        free(current);
    }
    free(list);
}

void intArrayOfListsFree(IntLinkedList** arrayOfLists, int size) {
    for (int i = 0; i < size; i++) {
        intLinkedListFree(arrayOfLists[i]);
    }
}

void intLinkedListInsertLast(IntLinkedList* list, int value) {
    Node* toInsert = (Node*)malloc(sizeof(Node));
    toInsert->key = value;
    toInsert->next = NULL;
    if (list != NULL) {
        if (list->first == NULL) {
            list->first = toInsert;
            list->last = toInsert;
        }
        else {
            list->last->next = toInsert;
            list->last = toInsert;
        }
        list->size = list->size + 1;
    }
}

void intLinkedListDeleteFirst(IntLinkedList* list) {
    Node* toDelet;
    if (list != NULL) {
        if (list->first != NULL) {
            toDelet = list->first;
            list->first = list->first->next;
            free(toDelet);
            if (list->first == NULL) {
                list->last = NULL;
            }
        }
    }
}

void intLinkedListPrint(IntLinkedList* list) {
    Node* current = list->first;
    printf("[");
    while (current != NULL) {
        printf(" %d", current->key);
        current = current->next;
    }
    printf(" ]");
}

void heapify(Node** arrayOfList, int position, int heap_size) {

    int smallest_value = position;
    int left_child = position * 2;
    int right_child = position * 2 + 1;

    if (right_child < heap_size && arrayOfList[right_child]->key < arrayOfList[position]->key) {
        smallest_value = right_child;
    }

    if (left_child < heap_size && arrayOfList[left_child]->key < arrayOfList[smallest_value]->key) {
        smallest_value = left_child;
    }

    if (smallest_value != position) {
        Node* aux = arrayOfList[position];
        arrayOfList[position] = arrayOfList[smallest_value];
        arrayOfList[smallest_value] = aux;

        heapify(arrayOfList, smallest_value, heap_size);
    }

}

void build_heap(Node** arrayOfList, int heap_size) {
    for (int i = heap_size / 2 - 1; i >= 0; i--) {
        heapify(arrayOfList, i, heap_size);
    }
}

void generateKSortedLists(IntLinkedList* arrayOfLists[], int k, int n) {
    int sizeOfEachList = n / k;
    int rest = n % k;
    int sizeOfLastList = rest + sizeOfEachList;
    for (int i = 0; i < k - 1; i++) {
        arrayOfLists[i] = intLinkedListNew();
        int array[10000];
        FillRandomArray(array, sizeOfEachList, 10, 50000, false, ASCENDING);

        for (int j = 0; j < sizeOfEachList; j++) {
            intLinkedListInsertLast(arrayOfLists[i], array[j]);
        }
    }

    int array[10000];
    arrayOfLists[k - 1] = intLinkedListNew();
    FillRandomArray(array, sizeOfLastList, 10, 50000, false, ASCENDING);
    for (int j = 0; j < sizeOfLastList; j++) {
        intLinkedListInsertLast(arrayOfLists[k - 1], array[j]);
    }
}

void merge2List(IntLinkedList** arrayOf2Lists, int sizeOfFisrt, int sizeOfSecound) {
    IntLinkedList* finalList = intLinkedListNew();

    int array1[100];
    arrayOf2Lists[0] = intLinkedListNew();
    FillRandomArray(array1, sizeOfFisrt, 10, 1000, false, ASCENDING);
    for (int i = 0; i < sizeOfFisrt; i++) {
        intLinkedListInsertLast(arrayOf2Lists[0], array1[i]);
    }
    printf("Prima lista :");
    intLinkedListPrint(arrayOf2Lists[0]);
    printf("\n");

    int array2[100];
    arrayOf2Lists[1] = intLinkedListNew();
    FillRandomArray(array2, sizeOfSecound, 10, 1000, false, ASCENDING);
    for (int i = 0; i < sizeOfSecound; i++) {
        intLinkedListInsertLast(arrayOf2Lists[1], array2[i]);
    }

    printf("Cea de a doua lista :");
    intLinkedListPrint(arrayOf2Lists[1]);
    printf("\n");
    int i = 0, j = 0;
    while (i < sizeOfFisrt && j < sizeOfSecound) {
        if (arrayOf2Lists[0]->first->key <= arrayOf2Lists[1]->first->key) {
            intLinkedListInsertLast(finalList, arrayOf2Lists[0]->first->key);
            intLinkedListDeleteFirst(arrayOf2Lists[0]);
            i++;
        }
        else {
            intLinkedListInsertLast(finalList, arrayOf2Lists[1]->first->key);
            intLinkedListDeleteFirst(arrayOf2Lists[1]);
            j++;
        }
    }

    if (i == sizeOfFisrt && j < sizeOfSecound) {
        while (j < sizeOfSecound) {
            intLinkedListInsertLast(finalList, arrayOf2Lists[1]->first->key);
            intLinkedListDeleteFirst(arrayOf2Lists[1]);
            j++;
        }
    }

    if (i < sizeOfFisrt && j == sizeOfSecound) {
        while (i < sizeOfFisrt) {
            intLinkedListInsertLast(finalList, arrayOf2Lists[0]->first->key);
            intLinkedListDeleteFirst(arrayOf2Lists[0]);
            i++;
        }
    }

    printf("Lista finala obtinuta :");
    intLinkedListPrint(finalList);
}

void generateArrayOfLists(IntLinkedList** multipleLists, Node** arrayOfLists, int k) {
    for (int i = 0; i < k; i++) {
        arrayOfLists[i] = (Node*)malloc(sizeof(Node));
        arrayOfLists[i] = multipleLists[i]->first;
    }
}

void kWayMerge(Node** arrayOfLists, int size) {
    IntLinkedList* finalList = intLinkedListNew();
    int heapSize = size;

    build_heap(arrayOfLists, size);
    while (heapSize > 0) {
        intLinkedListInsertLast(finalList, arrayOfLists[0]->key);
        arrayOfLists[0] = arrayOfLists[0]->next;

        if (arrayOfLists[0] == NULL) {
            arrayOfLists[0] = arrayOfLists[heapSize - 1];
            heapSize = heapSize - 1;
        }
        heapify(arrayOfLists, 0, heapSize);
    }
    printf("Sirul sortat este : ");
    intLinkedListPrint(finalList);
}
void demo() {
    IntLinkedList* arrayOfLists1[2];
    int sizeOfFisrt = 10;
    int sizeOfSecound = 6;
    merge2List(arrayOfLists1, sizeOfFisrt, sizeOfSecound);
    printf("\n");

    int k = 4;
    int n = 20;
    IntLinkedList* multipleLists[100];
    Node* arrayOfLists[100];

    generateKSortedLists(multipleLists, k, n);

    for (int i = 0; i < k; i++) {
        intLinkedListPrint(multipleLists[i]);
        printf("\n");
    }

    printf("\n");

    generateArrayOfLists(multipleLists, arrayOfLists, k);

    printf("Array-ul de liste generat : ");
    for (int i = 0; i < k; i++) {
        printf("%d ", arrayOfLists[i]->key);
    }
    printf("\n");
    kWayMerge(arrayOfLists, k);
    intArrayOfListsFree(multipleLists, k);
}

void heapify_test(Node** arrayOfList, int position, int heap_size, Operation* opkWayMerge) {

    int smallest_value = position;
    int left_child = position * 2;
    int right_child = position * 2 + 1;

    opkWayMerge->count();
    if ((right_child < heap_size) && (arrayOfList[right_child]->key < arrayOfList[position]->key)) {
        smallest_value = right_child;
    }

    opkWayMerge->count();
    if ((left_child < heap_size) && (arrayOfList[left_child]->key < arrayOfList[smallest_value]->key)) {
        smallest_value = left_child;
    }

    if (smallest_value != position) {
        opkWayMerge->count(3);

        Node* aux = arrayOfList[position];
        arrayOfList[position] = arrayOfList[smallest_value];
        arrayOfList[smallest_value] = aux;

        heapify_test(arrayOfList, smallest_value, heap_size, opkWayMerge);
    }

}

void build_heap_test(Node** arrayOfList, int heap_size, Operation* opkWayMerge) {
    for (int i = heap_size / 2 - 1; i >= 0; i--) {
        heapify_test(arrayOfList, i, heap_size, opkWayMerge);
    }
}

void kWayMergeTest(Node** arrayOfLists, int size, Operation* opkWayMerge) {
    IntLinkedList* finalList = intLinkedListNew();
    int heapSize = size;

    build_heap_test(arrayOfLists, size, opkWayMerge);
    while (heapSize > 0) {
        opkWayMerge->count(2);
        intLinkedListInsertLast(finalList, arrayOfLists[0]->key);
        arrayOfLists[0] = arrayOfLists[0]->next;

        opkWayMerge->count();
        if (arrayOfLists[0] == NULL) {
            opkWayMerge->count();
            arrayOfLists[0] = arrayOfLists[heapSize - 1];
            heapSize = heapSize - 1;
        }
        heapify_test(arrayOfLists, 0, heapSize, opkWayMerge);
    }
}

void test() {
    int k1 = 5;
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
            IntLinkedList* multipleLists[10000];
            Node* arrayOfLists[10000];

            generateKSortedLists(multipleLists, k1, n);
            generateArrayOfLists(multipleLists, arrayOfLists, k1);

            Operation opkWayMerge1 = p.createOperation("kWayMergeK1=5", n);

            kWayMergeTest(arrayOfLists, k1, &opkWayMerge1);

            intArrayOfListsFree(multipleLists, k1);
    }

    int k2 = 10;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        IntLinkedList* multipleLists[10000];
        Node* arrayOfLists[10000];

        generateKSortedLists(multipleLists, k2, n);
        generateArrayOfLists(multipleLists, arrayOfLists, k2);

        Operation opkWayMerge2 = p.createOperation("kWayMergeK2=10", n);

        kWayMergeTest(arrayOfLists, k2, &opkWayMerge2);

        intArrayOfListsFree(multipleLists, k2);
    }

    int k3 = 100;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        IntLinkedList* multipleLists[10000];
        Node* arrayOfLists[10000];

        generateKSortedLists(multipleLists, k3, n);
        generateArrayOfLists(multipleLists, arrayOfLists, k3);

        Operation opkWayMerge2 = p.createOperation("kWayMergeK3=100", n);

        kWayMergeTest(arrayOfLists, k3, &opkWayMerge2);

        intArrayOfListsFree(multipleLists, k3);
    }
    
    int n2 = 100000;
    for (int k = 10; k < 500; k += 10) {
        IntLinkedList* multipleLists[10000];
        Node* arrayOfLists[10000];

        generateKSortedLists(multipleLists, k, n2);
        generateArrayOfLists(multipleLists, arrayOfLists, k);

        Operation opkWayMergeN10000 = p.createOperation("kWayMergeN=10000", k);

        kWayMergeTest(arrayOfLists, k, &opkWayMergeN10000);

        intArrayOfListsFree(multipleLists, k);
    }

    p.createGroup("kWayMerge_K_Variance", "kWayMergeK1=5", "kWayMergeK2=10", "kWayMergeK3=100");
    p.showReport();
}

int main() {
    //demo();
    test();
    return 0;
}