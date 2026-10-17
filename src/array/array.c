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
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_INITIAL_CAPACITY 4

// Grows the backing buffer by doubling its capacity when it is full.
// Returns 1 on success (or when there is nothing to do), 0 on failure.
static int arr_grow(struct Array *a) {
	if (a->size < a->capacity) {
		return 1;
	}

	unsigned int new_capacity = a->capacity * 2;
	void *new_data = realloc(a->data, new_capacity * sizeof(void *));
	if (new_data == NULL) {
		printf("Error: malloc failed\n");
		return 0;
	}

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
