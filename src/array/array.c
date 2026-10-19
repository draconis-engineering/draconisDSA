/*
 * Dynamic Array Library - DRACONIS ENGINEERING DATA STRUCTURES &
 * ALGORITHMS Copyright (C) 2026 Simon Stordal Amundgård
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses.
 */

#include "array.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

constexpr int ARRAY_INITIAL_CAPACITY = 4;

// Grows the backing buffer by doubling its capacity when it is full.
// Returns 1 on success (or when there is nothing to do), 0 on failure.
static int arr_grow(struct Array *a) {

	// Do nothing if we still have capacity
	if (a->size < a->capacity) {
		return 1;
	}

	// Handle an initial capacity of 0, otherwise double it
	size_t new_capacity = (a->capacity == 0) ? 4 : a->capacity * 2;

	// Overflow protection check
	if (new_capacity < a->capacity ||
		new_capacity > (size_t)-1 / sizeof(void *)) {
		fprintf(stderr, "Error: Array capacity overflow\n");
		return 0;
	}

	// Safely reallocate using the temporary pointer
	void *new_data = realloc(a->data, new_capacity * sizeof(void *));
	if (new_data == NULL) {
		fprintf(stderr, "Error: realloc failed to expand buffer\n");
		return 0;
	}

	// Commit the changes
	a->data = new_data;
	a->capacity = new_capacity;
	return 1;
}

// Initializes the array with an empty buffer.
struct Array *create_array() {
	struct Array *a = (struct Array *)malloc(sizeof(struct Array));
	if (a == NULL) {
		printf("Error: malloc failed\n");
		return NULL;
	}

	// Allocate the initial buffer of void* elements.
	a->data = malloc(ARRAY_INITIAL_CAPACITY * sizeof(void *));
	if (a->data == NULL) {
		printf("Error: malloc failed\n");
		free(a);
		return NULL;
	}

	a->size = 0;
	a->capacity = ARRAY_INITIAL_CAPACITY;
	return a;
}

// Returns the number of stored elements.
int arr_size(struct Array *a) { return (int)a->size; }

// Returns whether the array holds no elements.
int arr_is_empty(struct Array *a) { return (a->size == 0); }

// Appends a value to the back of the array.
void arr_push(struct Array *a, void *val) {
	if (!arr_grow(a)) {
		return;
	}

	((void **)a->data)[a->size] = val;
	a->size++;
}

// Inserts a value at the given index. Valid indices are 0..size.
void arr_insert(struct Array *a, void *val, int idx) {
	int size = (int)a->size;
	if (idx < 0 || idx > size) {
		printf("Array index out of range!\n");
		return;
	}

	if (!arr_grow(a)) {
		return;
	}

	void **data = (void **)a->data;

	// Shift elements at idx..size-1 one slot to the right.
	for (int i = size; i > idx; i--) {
		data[i] = data[i - 1];
	}

	data[idx] = val;
	a->size++;
}

// Returns the value stored at the given index, or NULL if out of range.
void *arr_get(struct Array *a, int idx) {
	if (idx < 0 || idx >= (int)a->size) {
		printf("Array index out of range!\n");
		return NULL;
	}

	return ((void **)a->data)[idx];
}

// Overwrites the value at the given index.
void arr_set(struct Array *a, int idx, void *val) {
	if (idx < 0 || idx >= (int)a->size) {
		printf("Array index out of range!\n");
		return;
	}

	((void **)a->data)[idx] = val;
}

// Removes the element at the given index, shifting later elements left.
void arr_remove(struct Array *a, int idx) {
	int size = (int)a->size;
	if (idx < 0 || idx >= size) {
		printf("Array index out of range!\n");
		return;
	}

	void **data = (void **)a->data;

	// Shift elements at idx+1..size-1 one slot to the left.
	for (int i = idx; i < size - 1; i++) {
		data[i] = data[i + 1];
	}

	a->size--;
}

// Removes and returns the last element, or NULL if the array is empty.
void *arr_pop(struct Array *a) {
	if (a->size == 0) {
		printf("Array is empty!\n");
		return NULL;
	}

	a->size--;
	return ((void **)a->data)[a->size];
}

// Prints the array to the console as pointer addresses.
void print_array(struct Array *a) {
	printf("[");
	for (unsigned int i = 0; i < a->size; i++) {
		printf("%p", ((void **)a->data)[i]);
		if (i + 1 < a->size) {
			printf(", ");
		}
	}

	printf("]\n");
}

// Frees the backing buffer and the array structure itself.
void free_array(struct Array *a) {
	free(a->data);
	free(a);
}

// ============================== \\
// ----- SORTING ALGORITHMS ----- \\
// ============================== \\

// Helper to compare two generic elements (Assumes integer values cast to void*)
static int default_compare(const void *a, const void *b) {
	return ((intptr_t)a > (intptr_t)b) - ((intptr_t)a < (intptr_t)b);
}

// Internal recursive worker that sorts directly within the data buffer
static void merge_sort_worker(void **arr, void **temp, unsigned int left,
							  unsigned int right) {
	if (left >= right) {
		return;
	}

	unsigned int mid = left + (right - left) / 2;

	// Conquer: Sort left and right halves
	merge_sort_worker(arr, temp, left, mid);
	merge_sort_worker(arr, temp, mid + 1, right);

	// Combine: Merge the sorted halves into temp
	unsigned int i = left;
	unsigned int j = mid + 1;
	unsigned int k = left;

	while (i <= mid && j <= right) {
		if (default_compare(arr[i], arr[j]) <= 0) {
			temp[k++] = arr[i++];
		} else {
			temp[k++] = arr[j++];
		}
	}

	// Copy remaining elements
	while (i <= mid) {
		temp[k++] = arr[i++];
	}
	while (j <= right) {
		temp[k++] = arr[j++];
	}

	// Copy back the merged elements into the original array
	for (i = left; i <= right; i++) {
		arr[i] = temp[i];
	}
}

// Merge sort: Divide, Conquer, Merge and Repeat
struct Array *arr_merge_sort(struct Array *a) {
	if (a == NULL || a->size <= 1 || a->data == NULL) {
		return a;
	}

	// Cast the void* data buffer to an actual array of void* pointers
	void **arr = (void **)a->data;

	// Allocate exactly ONE temporary buffer for the entire lifetime of the sort
	void **temp = (void **)malloc(a->size * sizeof(void *));
	if (temp == NULL) {
		return NULL; // Handle allocation failure safely
	}

	// Run the in-place merge sort
	merge_sort_worker(arr, temp, 0, a->size - 1);

	// Clean up temporary buffer
	free(temp);

	return a;
}
