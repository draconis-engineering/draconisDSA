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

#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stddef.h>

#include "shared/shared.h"

/**
 * @brief A binary heap of `void*` payloads.
 *
 * The heap is a complete binary tree kept in a flat array: the children of
 * the element at index `i` live at `2 * i + 1` and `2 * i + 2`, so the
 * element at index 0 is the top of the heap.
 *
 * The order is decided by the comparator. With the usual "smaller comes
 * first" comparator the heap behaves as a min-heap, and `heap_pop()` returns
 * the smallest element. Flipping the comparator turns it into a max-heap.
 *
 * The payloads are owned by the caller; only the internal buffer is freed.
 */
struct Heap {
	/** The elements in level order. */
	void **items;
	/** Number of elements currently in the heap. */
	size_t size;
	/** Number of elements the buffer can hold before it has to grow. */
	size_t capacity;
	/** Comparator that decides the order of the heap. */
	cmp_fn cmp;
};

/**
 * @brief Create an empty heap.
 * @param cmp Comparator that decides the order. Must not be NULL.
 * @return A pointer to the new heap, or NULL on failure or if cmp is NULL.
 */
struct Heap *create_heap(cmp_fn cmp);

/**
 * @brief Insert an element into the heap.
 * @param heap Pointer to the heap.
 * @param item The element to insert.
 * @return True if the element was inserted, false on allocation failure.
 */
bool heap_push(struct Heap *heap, void *item);

/**
 * @brief Remove the top element from the heap and return it.
 * @param heap Pointer to the heap.
 * @return The removed element, or NULL if the heap was empty.
 */
void *heap_pop(struct Heap *heap);

/**
 * @brief Look at the top element without removing it.
 * @param heap Pointer to the heap.
 * @return The top element, or NULL if the heap was empty.
 */
void *heap_peek(const struct Heap *heap);

/**
 * @brief Get the number of elements in the heap.
 * @param heap Pointer to the heap.
 * @return Number of elements.
 */
size_t heap_size(const struct Heap *heap);

/**
 * @brief Check whether the heap holds no elements.
 * @param heap Pointer to the heap.
 * @return True if empty, false otherwise.
 */
bool heap_is_empty(const struct Heap *heap);

/**
 * @brief Free the heap's buffer and the heap itself. NULL-safe.
 * @param heap Pointer to the heap.
 */
void free_heap(struct Heap *heap);

#endif // HEAP_H
