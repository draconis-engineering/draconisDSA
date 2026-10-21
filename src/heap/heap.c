/*
 * Binary Heap Library - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
 * Copyright (C) 2026 Simon Stordal Amundgård
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

#include "heap.h"

#include <stdio.h>
#include <stdlib.h>

static const size_t HEAP_INITIAL_CAPACITY = 4;

// Grows the backing buffer by doubling its capacity when it is full.
static int heap_grow(struct Heap *heap) {
	if (heap->size < heap->capacity) {
		return 1;
	}

	size_t new_capacity = (heap->capacity == 0) ? HEAP_INITIAL_CAPACITY
												 : heap->capacity * 2;

	if (new_capacity < heap->capacity ||
		new_capacity > (size_t)-1 / sizeof(void *)) {
		fprintf(stderr, "Error: Heap capacity overflow\n");
		return 0;
	}

	void *new_items = realloc(heap->items, new_capacity * sizeof(void *));
	if (new_items == NULL) {
		fprintf(stderr, "Error: realloc failed to expand buffer\n");
		return 0;
	}

	heap->items = (void **)new_items;
	heap->capacity = new_capacity;
	return 1;
}

// Exchanges two elements of the backing array.
static void heap_swap(void **items, size_t a, size_t b) {
	void *tmp = items[a];
	items[a] = items[b];
	items[b] = tmp;
}

// Moves the element at `index` up until its parent is in the right order.
static void heap_sift_up(struct Heap *heap, size_t index) {
	while (index > 0) {
		size_t parent = (index - 1) / 2;
		if (heap->cmp(heap->items[index], heap->items[parent]) >= 0) {
			break;
		}
		heap_swap(heap->items, index, parent);
		index = parent;
	}
}

// Moves the element at `index` down until both of its children are in the
// right order.
static void heap_sift_down(struct Heap *heap, size_t index) {
	for (;;) {
		size_t left = 2 * index + 1;
		size_t right = 2 * index + 2;
		size_t smallest = index;

		if (left < heap->size &&
			heap->cmp(heap->items[left], heap->items[smallest]) < 0) {
			smallest = left;
		}
		if (right < heap->size &&
			heap->cmp(heap->items[right], heap->items[smallest]) < 0) {
			smallest = right;
		}

		if (smallest == index) {
			return;
		}

		heap_swap(heap->items, index, smallest);
		index = smallest;
	}
}

// Creates a heap with an empty buffer.
struct Heap *create_heap(cmp_fn cmp) {
	if (cmp == NULL) {
		fprintf(stderr, "Error: a heap needs a comparator\n");
		return NULL;
	}

	struct Heap *heap = (struct Heap *)malloc(sizeof(struct Heap));
	if (heap == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return NULL;
	}

	heap->items = (void **)malloc(HEAP_INITIAL_CAPACITY * sizeof(void *));
	if (heap->items == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		free(heap);
		return NULL;
	}

	heap->size = 0;
	heap->capacity = HEAP_INITIAL_CAPACITY;
	heap->cmp = cmp;
	return heap;
}

// Inserts an element and restores the heap order.
bool heap_push(struct Heap *heap, void *item) {
	if (heap == NULL) {
		return false;
	}

	if (!heap_grow(heap)) {
		return false;
	}

	heap->items[heap->size++] = item;
	heap_sift_up(heap, heap->size - 1);
	return true;
}

// Removes the top element and restores the heap order.
void *heap_pop(struct Heap *heap) {
	if (heap == NULL || heap->size == 0) {
		return NULL;
	}

	void *top = heap->items[0];
	heap->size--;

	if (heap->size > 0) {
		heap->items[0] = heap->items[heap->size];
		heap_sift_down(heap, 0);
	}

	return top;
}

// Returns the top element without removing it.
void *heap_peek(const struct Heap *heap) {
	if (heap == NULL || heap->size == 0) {
		return NULL;
	}
	return heap->items[0];
}

// Returns the number of elements in the heap.
size_t heap_size(const struct Heap *heap) {
	return (heap != NULL) ? heap->size : 0;
}

// Returns whether the heap holds no elements.
bool heap_is_empty(const struct Heap *heap) {
	return heap_size(heap) == 0;
}

// Frees the buffer and the heap itself.
void free_heap(struct Heap *heap) {
	if (heap == NULL) {
		return;
	}

	free(heap->items);
	free(heap);
}
