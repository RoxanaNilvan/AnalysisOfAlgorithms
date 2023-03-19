/*Nilvan Roxana Maria 
* Grupa 30228
* Am implementat doi algoritmi pentru construirea unui heap.
* Cele doua strategii implementate sunt "Top Down" si "Bottom up"
* Dupa implementarea celor doi algoritmi am realizat ca amandoi au complexitate O(n)
* Cel mai bun dintre cei doi algoritmi este "Top Down", doarece atat in caz mediu statistic cat si in cel mai defavorabil caz are comportament mai bun
*/

#include<stdio.h>
#include "Profiler.h"

Profiler p("heapAlgorithms");

#define MIN_VALUE -300000000

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

void heapify(int heap[], int position, int heap_size, Operation opBottomUp) {

	int largest_value = position;
	int left_child = position * 2  ;
	int right_child = position * 2 + 1;
	
	opBottomUp.count();
	if (right_child < heap_size && heap[largest_value] < heap[right_child]) {
		largest_value = right_child;
	}

	opBottomUp.count();
	if (left_child < heap_size && heap[largest_value] < heap[left_child]) {
		largest_value = left_child;
	}

	if (largest_value != position) {
		opBottomUp.count(3);
		int aux = heap[position];
		heap[position] = heap[largest_value];
		heap[largest_value] = aux;

		heapify(heap, largest_value, heap_size, opBottomUp);
	}

}

void build_heap(int heap[], int heap_size) {
	Operation opBottomUp = p.createOperation("BottomUp.operations", heap_size);
	for (int i = heap_size / 2 - 1; i >=  0; i--) {
		heapify(heap, i, heap_size, opBottomUp);
	}
}

void heap_sort(int heap[], int heap_size) {
	Operation opHeapSort = p.createOperation("HeapSort", heap_size);
	build_heap(heap, heap_size);

	for (int i = heap_size - 1; i >= 1; i--) {
		int aux = heap[0];
		heap[0] = heap[i];
		heap[i] = aux;

		heap_size = heap_size - 1;
		heapify(heap, 0, heap_size, opHeapSort);
	}
}

//Cormen problema 6.5 si 6-1
void heap_increase_key(int heap[], int position, int key, int array_length, Operation opTopDown) {

	opTopDown.count();
	if (key < heap[position]) {
		printf("New key is smaller than current key");
		return;
	}

	opTopDown.count();
	heap[position] = key;

	while ((position > 0) && (heap[position / 2] < heap[position])) {
		opTopDown.count();

		opTopDown.count(3);
		int aux = heap[position / 2];
		heap[position / 2] = heap[position];
		heap[position] = aux;
		position = position / 2;
	}
}

void max_heap_insert(int heap[], int *heap_size,  int key, int array_lenght, Operation opTopDown) {
	*heap_size = *heap_size + 1;
	heap[*heap_size] = MIN_VALUE;
	heap_increase_key(heap, *heap_size, key, array_lenght, opTopDown);
}

void build_max_heap(int array[], int array_lenght, int heap[]) {
	Operation opTopDown = p.createOperation("TopDown.operations", array_lenght);

	int heap_size = 0;

	opTopDown.count();
	heap[0] = array[0];
	for (int i = 1; i < array_lenght; i++) {
		max_heap_insert(heap, &heap_size, array[i], array_lenght, opTopDown);
	}
}


void demo() {
	int array[] = { 10, 12, 13, 6, 7, 5 };
	int lenght = sizeof(array) / sizeof(array[0]);

	printf("Bottom up : ");
	build_heap(array, lenght);
	for (int i = 0; i < lenght; i++) {
		printf("%d ", array[i]);
	}

	printf("\n");

	printf("Top down : ");
	int array2[] = { 10, 12, 13, 6, 7, 5 };
	int lenght2 = sizeof(array) / sizeof(array[0]);
	int heap_formed[6];
	build_max_heap(array2, lenght2, heap_formed);

	for (int i = 0; i < lenght; i++) {
		printf("%d ",heap_formed[i]);
	}

	printf("\nHeapul sortat este : ");
	heap_sort(array, lenght);
	for (int i = 0; i < lenght; i++) {
		printf("%d ", array[i]);
	}
}

void test_average(int order) {
	int v[MAX_SIZE], v2[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		for (int test = 0; test < NR_TESTS; test++) {
			FillRandomArray(v, n, 10, 50000, false, order);
			for (int i = 0; i < n; i++) {
				v2[i] = v[i];
			}

			int heap_generated[MAX_SIZE];
			build_heap(v, n);
			build_max_heap(v2, n, heap_generated);

			p.divideValues("opBottomUp",  NR_TESTS);

			p.divideValues("opTopDown", NR_TESTS);

			p.createGroup("heapComparison", "TopDown.operations", "BottomUp.operations");
			
		}
	}
	
}

void test_1() {
	test_average(UNSORTED);
	p.showReport();
}

void test_worst(int order) {
	int v[MAX_SIZE], v2[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
			FillRandomArray(v, n, 10, 50000, false, order);
			for (int i = 0; i < n; i++) {
				v2[i] = v[i];
			}

			int heap_generated[MAX_SIZE];
			build_heap(v, n);
			build_max_heap(v2, n, heap_generated);

			p.createGroup("heapComparison", "TopDown.operations", "BottomUp.operations");

		}
	}


void test_2() {
	test_worst(ASCENDING);
	p.showReport();
}



int main() {
	demo();
	//test_1();
	//test_2();
	return 0;
}