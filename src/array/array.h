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

#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

/**
 * @brief A growable, contiguous array of void* elements.
 *
 * The buffer doubles in capacity whenever it fills up, giving amortized
 * O(1) appends. Elements are stored as `void*` and owned by the caller
 * (only the internal buffer is freed, never the payloads).
 */
struct Array {
	/** Number of elements currently stored. */
	unsigned int size;
	/** Number of elements the buffer can hold before it must grow. */
	unsigned int capacity;
	/** Pointer to the contiguous buffer of `void*` elements. */
	void *data;
};

/**
 * @brief Create and initialize an empty array.
 * @return A pointer to the new array, or NULL on allocation failure.
 */
struct Array *create_array();

/**
 * @brief Get the number of elements in the array.
 * @param a Pointer to the array.
 * @return Size of the array.
 */
int arr_size(struct Array *a);

/**
 * @brief Check if the array is empty.
 * @param a Pointer to the array.
 * @return 1 if empty, 0 otherwise.
 */
int arr_is_empty(struct Array *a);

/**
 * @brief Append a value to the back of the array, growing if needed.
 * @param a Pointer to the array.
 * @param val Pointer to the value to append.
 */
void arr_push(struct Array *a, void *val);

/**
 * @brief Insert a value at the specified index, shifting later elements right.
 * @param a Pointer to the array.
 * @param val Pointer to the value to insert.
 * @param idx Index to insert at (0 <= idx <= size).
 */
void arr_insert(struct Array *a, void *val, int idx);

/**
 * @brief Get the value stored at an index.
 * @param a Pointer to the array.
 * @param idx Index to read (0 <= idx < size).
 * @return The stored pointer, or NULL if the index is out of range.
 */
void *arr_get(struct Array *a, int idx);

/**
 * @brief Overwrite the value at an index.
 * @param a Pointer to the array.
 * @param idx Index to write (0 <= idx < size).
 * @param val Pointer to the value to store.
 */
void arr_set(struct Array *a, int idx, void *val);

/**
 * @brief Remove the element at an index, shifting later elements left.
 * @param a Pointer to the array.
 * @param idx Index to remove (0 <= idx < size).
 */
void arr_remove(struct Array *a, int idx);

/**
 * @brief Remove and return the last element.
 * @param a Pointer to the array.
 * @return The removed pointer, or NULL if the array is empty.
 */
void *arr_pop(struct Array *a);

/**
 * @brief Print the array contents as pointer addresses.
 * @param a Pointer to the array.
 */
void print_array(struct Array *a);

/**
 * @brief Free the array's buffer and the array structure itself.
 * @param a Pointer to the array.
 */
void free_array(struct Array *a);

#endif // ARRAY_H
