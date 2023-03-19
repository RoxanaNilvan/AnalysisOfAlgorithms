/*Nilvan Roxana Maria 
* Grupa 30228
* In aceasta tema am implamentat metoda de sortare QuickSort si am realizat o comparatie intre aceasta si HeapSort
* Dupa implementarea celor doua metode am constatat ca in caz mediu statistic HeapSort are un compotament mai bun decat QuickSort, dar
* QuickSort este mai rapdi si are perfromanta mai buna atat in caz defavorabil cat si in caz favorabil.
* Tot din masuratorile realizate am observat ca selection sort recursiv si selection sort iterativ au aceeasi complexitate si ca timpii de rulare
* sunt asemanatori in caz mediu statistic.
* Quick Sort are complexitate nlogn in best case, in caz mediu statistic complexitatea se apropie de best case, iar in worst case complexitatea este de
* n^2
*/
#include<stdio.h>
#include"Profiler.h"
Profiler p("sorting");

#define MIN_VALUE -3000000000
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5
#define TIME_TESTS 500

// heap sort
void heapify(int heap[], int position, int heap_size, Operation opHeapSort) {

	int largest_value = position;
	int left_child = position * 2;
	int right_child = position * 2 + 1;

	opHeapSort.count();
	if (right_child < heap_size && heap[largest_value] < heap[right_child]) {
		largest_value = right_child;
	}

	opHeapSort.count();
	if (left_child < heap_size && heap[largest_value] < heap[left_child]) {
		largest_value = left_child;
	}

	if (largest_value != position) {

		opHeapSort.count(3);
		int aux = heap[position];
		heap[position] = heap[largest_value];
		heap[largest_value] = aux;

		heapify(heap, largest_value, heap_size, opHeapSort);
	}

}

void build_heap(int heap[], int heap_size, Operation opHeapSort) {
	for (int i = heap_size / 2 - 1; i >= 0; i--) {
		heapify(heap, i, heap_size, opHeapSort);
	}
}

void heap_sort(int heap[], int heap_size) {
	Operation opHeapSort = p.createOperation("HeapSortOparations", heap_size);

	build_heap(heap, heap_size, opHeapSort);

	for (int i = heap_size - 1; i >= 1; i--) {
		opHeapSort.count(3);
		int aux = heap[0];
		heap[0] = heap[i];
		heap[i] = aux;

		heap_size = heap_size - 1;
		heapify(heap, 0, heap_size, opHeapSort);
	}
}
// quick sort demo
int partition(int array[], int left_index, int right_index)
{
	int pivot = array[right_index];
	int i = (left_index - 1);

	for (int j = left_index; j < right_index; j++)
	{
		if (array[j] <= pivot)
		{
			i++;
			int aux = array[i];
			array[i] = array[j];
			array[j] = aux;
		}
	}
	int aux = array[right_index];
	array[right_index] = array[i + 1];
	array[i + 1] = aux;
	return (i + 1);
}

void quickSort(int array[], int left_index, int right_index)
{
	if (left_index < right_index) {
		int index_of_partition = partition(array, left_index, right_index);
		quickSort(array, left_index, index_of_partition - 1);
		quickSort(array, index_of_partition + 1, right_index);
	}
}

int quickselect(int array[], int left_index, int right_index, int desired_rank) {
	if (desired_rank > 0 && desired_rank <= right_index - left_index + 1) {

		int partition_index = partition(array, left_index, right_index);

		if (partition_index - left_index == desired_rank - 1) {
			return array[partition_index];
		}

		if (partition_index - left_index > desired_rank - 1) {
			return quickselect(array, left_index, partition_index - 1, desired_rank);
		}
		else {
			return quickselect(array, partition_index + 1, right_index, desired_rank - partition_index + left_index - 1);
		}
	}
	else return MIN_VALUE;
}

int partition_test(int array[], int left_index, int right_index, Operation* opQuickSort)
{
	opQuickSort->count();
	int pivot = array[right_index];
	int i = (left_index - 1);

	for (int j = left_index; j < right_index; j++)
	{
		opQuickSort->count();
		if (array[j] <= pivot)
		{
			i++;
			opQuickSort->count(3);
			int aux = array[i];
			array[i] = array[j];
			array[j] = aux;
		}
	}
	opQuickSort->count(3);
	int aux = array[right_index];
	array[right_index] = array[i + 1];
	array[i + 1] = aux;
	return (i + 1);
}

void quick_sort_test(int arr[], int left_index, int right_index, Operation* opQuickSort)
{
	if (left_index < right_index) {
		int index_of_partition = partition_test(arr, left_index, right_index, opQuickSort);
		quick_sort_test(arr, left_index, index_of_partition - 1, opQuickSort);
		quick_sort_test(arr, index_of_partition + 1, right_index, opQuickSort);
	}
}

int partitionBest(int array[], int left_index, int right_index)
{
	int pivot = array[(right_index + left_index) / 2];
	int i = (left_index - 1);

	for (int j = left_index; j < right_index; j++)
	{
		if (array[j] <= pivot)
		{
			i++;
			int aux = array[i];
			array[i] = array[j];
			array[j] = aux;
		}
	}
	int aux = array[right_index];
	array[right_index] = array[i + 1];
	array[i + 1] = aux;
	return (i + 1);
}

void quickSortBest(int array[], int left_index, int right_index)
{
	if (left_index < right_index) {
		int index_of_partition = partitionBest(array, left_index, right_index);
		quickSortBest(array, left_index, index_of_partition - 1);
		quickSortBest(array, index_of_partition + 1, right_index);
	}
}

int partitionBestTest(int array[], int left_index, int right_index, Operation* opQuickSortBest)
{
	opQuickSortBest->count();
	int pivot = array[(right_index + left_index) / 2];
	int i = (left_index - 1);

	for (int j = left_index; j < right_index; j++)
	{
		opQuickSortBest->count();
		if (array[j] <= pivot)
		{
			i++;
			opQuickSortBest->count(3);
			int aux = array[i];
			array[i] = array[j];
			array[j] = aux;
		}
	}
	opQuickSortBest->count(3);
	int aux = array[right_index];
	array[right_index] = array[i + 1];
	array[i + 1] = aux;
	return (i + 1);
}

void quickSortBestTest(int array[], int left_index, int right_index, Operation* opQuickSortBest)
{
	if (left_index < right_index) {
		int index_of_partition = partitionBestTest(array, left_index, right_index, opQuickSortBest);
		quickSortBestTest(array, left_index, index_of_partition - 1, opQuickSortBest);
		quickSortBestTest(array, index_of_partition + 1, right_index, opQuickSortBest);
	}
}

void selection_sort(int vector[], int lenght) {
	Operation opSelectionIterativ = p.createOperation("selectionIterativ", lenght);

	int min_poz;
	for (int i = 0; i < lenght - 1; i++) {
		min_poz = i;
		for (int j = i + 1; j < lenght; j++) {
			opSelectionIterativ.count();
			if (vector[j] < vector[min_poz]) {
				min_poz = j;
			}
		}

		if (i != min_poz) {
			opSelectionIterativ.count(3);
			int aux = vector[i];
			vector[i] = vector[min_poz];
			vector[min_poz] = aux;
		}
	}
}

void recursive_selection_sort_demo(int vector[], int length) {
	if (length <= 1) {
		return;
	}

	int max_value = vector[0];
	int index_of_max = 0;
	for (int i = 1; i < length; i++) {

		if (vector[i] > max_value) {

			max_value = vector[i];
			index_of_max = i;
		}
	}


	if (index_of_max != length - 1) {
		int aux = vector[index_of_max];
		vector[index_of_max] = vector[length - 1];
		vector[length - 1] = aux;
	}

	recursive_selection_sort_demo(vector, length - 1);
}

void recursive_selection_sort(int vector[], int length, Operation* opSelectionRecursiv) {
	if (length <= 1) {
		return;
	}

	opSelectionRecursiv->count();
	int max_value = vector[0];
	int index_of_max = 0;
	for (int i = 1; i < length; i++) {
		opSelectionRecursiv->count();
		if (vector[i] > max_value) {
			opSelectionRecursiv->count();
			max_value = vector[i];
			index_of_max = i;
		}
	}

	if (index_of_max != length - 1) {
		opSelectionRecursiv->count(3);
		int aux = vector[index_of_max];
		vector[index_of_max] = vector[length - 1];
		vector[length - 1] = aux;
	}

	recursive_selection_sort(vector, length - 1, opSelectionRecursiv);
}

void test_1(int order) {
	int array1[MAX_SIZE], array2[MAX_SIZE];
	int n;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {

		for (int test = 0; test < NR_TESTS; test++) {
			FillRandomArray(array1, n, 10, 50000, false, order);
			for (int i = 0; i < n; i++) {
				array2[i] = array1[i];
			}
			Operation opQuickSort = p.createOperation("QuickSortOperation", n);
			heap_sort(array1, n);
			quick_sort_test(array2, 0, n - 1, &opQuickSort);
		}
	}

	p.divideValues("HeapSortOparations", NR_TESTS);
	p.divideValues("QuickSortOperation", NR_TESTS);

	p.createGroup("averageCase", "HeapSortOparations", "QuickSortOperation");
}

void test_average() {
	test_1(UNSORTED);
	p.showReport();
}

void test_2(int order) {
	int array1[MAX_SIZE];
	int n;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {

		for (int test = 0; test < NR_TESTS; test++) {
			FillRandomArray(array1, n, 10, 50000, false, order);

			Operation opQuickSort = p.createOperation("QuickSortOperation", n);
			quick_sort_test(array1, 0, n - 1, &opQuickSort);
		}
	}
	p.divideValues("QuickSortOperation", NR_TESTS);

}

void test_worst() {
	test_2(ASCENDING);
	p.showReport();
}

void tests_selection(int order) {
	int v[MAX_SIZE], v2[MAX_SIZE], v3[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
		for (int test = 0; test < NR_TESTS; test++) {
			FillRandomArray(v, n, 10, 50000, false, order);
			for (int i = 0; i < n; i++) {
				v2[i] = v[i];
			}

			selection_sort(v, n);
			Operation opSelectionRecursiv = p.createOperation("selectionRecursiv", n);

			recursive_selection_sort(v2, n, &opSelectionRecursiv);

		}
	}
	
	p.divideValues("selectionIterativ", NR_TESTS);

	p.divideValues("selectionRecursiv", NR_TESTS);

	p.createGroup("selection", "selectionIterativ", "selectionRecursiv");
	
}

void test_3() {
	tests_selection(UNSORTED);
	p.showReport();
}

void time() {
	p.reset("recursiv-vs-iterativ");
	const int step = 100;
	const int time_test = 100;
	const int max_size2 = step * 100;

	int vector1[max_size2], vector2[max_size2], vector3[max_size2], vector4[max_size2];
	FillRandomArray(vector1, max_size2);
	CopyArray(vector2, vector1, max_size2);
	CopyArray(vector3, vector1, max_size2);
	CopyArray(vector4, vector1, max_size2);

	for (int n = step; n < max_size2; n += step) {
		selection_sort(vector1, n);

		Operation recursiv = p.createOperation("selection_sort_recursiv", n);
		recursive_selection_sort(vector2, n, &recursiv);
	}

	for (int n = step; n <= max_size2; n += step) {
		p.startTimer("selection_sort_iterartiv", n);
		for (int test = 0; test < time_test; test++) {
			selection_sort(vector3, n);
		}
		p.stopTimer("selection_sort_iterartiv", n);

		Operation recursiv = p.createOperation("selection_sort_recursiv", n);
		p.startTimer("selection_sort_recursiv", n);
		for (int test = 0; test < time_test; test++) {
			recursive_selection_sort(vector4, n, &recursiv);
		}
		p.stopTimer("selection_sort_recursiv", n);
	}

	p.createGroup("selection_sort", "selection_sort_iterativ", "selection_sort_recursiv");
	p.showReport();
}

void test_4(int order) {
	int array1[MAX_SIZE], array2[MAX_SIZE];
	int n;

	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {

		for (int test = 0; test < NR_TESTS; test++) {
			FillRandomArray(array1, n, 10, 50000, false, order);
			for (int i = 0; i < n; i++) {
				array2[i] = array1[i];
			}
			Operation opQuickSortBest = p.createOperation("QuickSortOperationBest", n);
			quickSortBestTest(array2, 0, n - 1, &opQuickSortBest);
		}
	}
	p.divideValues("QuickSortOperationBest", NR_TESTS);
}

void test_best_case() {
	test_4(ASCENDING);
	p.showReport();
}
void demo() {
	int array_quicksort[] = { 10, 12, 13, 6, 7, 5 };
	int length_quicksort = sizeof(array_quicksort) / sizeof(array_quicksort[0]);

	quickSort(array_quicksort, 0, length_quicksort - 1);

	printf("Sirul rezultat prin quicksort : ");
	for (int i = 0; i < length_quicksort; i++) {
		printf("%d ", array_quicksort[i]);
	}

	printf("\n");

	int array_quicksort_best[] = { 10, 12, 13, 6, 7, 5 };
	int length_quicksort_best = sizeof(array_quicksort_best) / sizeof(array_quicksort_best[0]);

	quickSort(array_quicksort_best, 0, length_quicksort_best - 1);

	printf("Sirul rezultat prin quicksort best : ");
	for (int i = 0; i < length_quicksort_best; i++) {
		printf("%d ", array_quicksort_best[i]);
	}

	printf("\n");

	int array_heapsort[] = { 10, 12, 13, 6, 7, 5 };
	int length_heapsort = sizeof(array_heapsort) / sizeof(array_heapsort[0]);

	heap_sort(array_heapsort, length_heapsort);

	printf("Sirul rezultat prin heapsort : ");
	for (int i = 0; i < length_heapsort; i++) {
		printf("%d ", array_heapsort[i]);
	}

	printf("\n");

	int array2[] = { 10, 4, 5, 8, 6, 11, 26 };
	int length2 = sizeof(array2) / sizeof(array2[0]);
	int searched_value = quickselect(array2, 0, length2 - 1, 3);
	printf("Valoarea cautata de QuickSelect : %d", searched_value);

	printf("\n");
	int selection_recursiv[] = { 7, 2, 8, 5, 9, 1, 6 };
	int selection_length = sizeof(selection_recursiv) / sizeof(selection_recursiv[0]);
	printf("Sirul rezultat prin selection sort recursiv : ");
	recursive_selection_sort_demo(selection_recursiv, selection_length);
	for (int i = 0; i < selection_length; i++) {
		printf("%d ", selection_recursiv[i]);
	}

	printf("\n");
	int selection[] = { 7, 2, 8, 5, 9, 1, 6 };
	int selection_length_int = sizeof(selection) / sizeof(selection[0]);
	printf("Sirul rezultat prin selection sort iterativ : ");
	selection_sort(selection, selection_length_int);
	for (int i = 0; i < selection_length_int; i++) {
		printf("%d ", selection[i]);
	}
}

int main() {
	demo();
	//test_average();
	//test_worst();
	//test_3();
	//time();
	//test_best_case();
	return 0;
}