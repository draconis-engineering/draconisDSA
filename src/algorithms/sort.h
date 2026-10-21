/*
 * Sorting Algorithms - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#ifndef SORT_H
#define SORT_H

#include <stdbool.h>
#include <stddef.h>

#include "shared/shared.h"

/**
 * @brief Sort `items` ascending with the bubble sort algorithm.
 *
 * Bubble sort repeatedly walks the array and swaps neighbours that are in the
 * wrong order. It is the slowest of the sorts here, but it is also the
 * easiest to read, which makes it a good starting point.
 *
 * @param items The array of elements to sort in place. May be NULL.
 * @param count Number of elements in `items`.
 * @param cmp Comparator deciding the order. Must not be NULL.
 * @return True if the array was sorted, false if the arguments were invalid.
 */
bool algo_bubble_sort(void **items, size_t count, cmp_fn cmp);

/**
 * @brief Sort `items` ascending with the selection sort algorithm.
 *
 * Selection sort repeatedly picks the smallest element that is still out of
 * place and swaps it into position.
 *
 * @param items The array of elements to sort in place. May be NULL.
 * @param count Number of elements in `items`.
 * @param cmp Comparator deciding the order. Must not be NULL.
 * @return True if the array was sorted, false if the arguments were invalid.
 */
bool algo_selection_sort(void **items, size_t count, cmp_fn cmp);

/**
 * @brief Sort `items` ascending with the insertion sort algorithm.
 *
 * Insertion sort grows a sorted prefix at the front of the array, one
 * element at a time. It is the fastest of the quadratic sorts on nearly
 * sorted input.
 *
 * @param items The array of elements to sort in place. May be NULL.
 * @param count Number of elements in `items`.
 * @param cmp Comparator deciding the order. Must not be NULL.
 * @return True if the array was sorted, false if the arguments were invalid.
 */
bool algo_insertion_sort(void **items, size_t count, cmp_fn cmp);

/**
 * @brief Sort `items` ascending with the quick sort algorithm.
 *
 * Quick sort picks a pivot, moves the elements that belong before it to its
 * left and the rest to its right, and then sorts both halves. It sorts in
 * place and in average O(n log n) time, but it needs O(log n) stack space.
 *
 * @param items The array of elements to sort in place. May be NULL.
 * @param count Number of elements in `items`.
 * @param cmp Comparator deciding the order. Must not be NULL.
 * @return True if the array was sorted, false if the arguments were invalid.
 */
bool algo_quick_sort(void **items, size_t count, cmp_fn cmp);

/**
 * @brief Sort `items` ascending with the heap sort algorithm.
 *
 * Heap sort first turns the array into a max-heap and then repeatedly swaps
 * the largest remaining element to the back. It sorts in place, always runs
 * in O(n log n) time and never needs more than O(1) extra space.
 *
 * @param items The array of elements to sort in place. May be NULL.
 * @param count Number of elements in `items`.
 * @param cmp Comparator deciding the order. Must not be NULL.
 * @return True if the array was sorted, false if the arguments were invalid.
 */
bool algo_heap_sort(void **items, size_t count, cmp_fn cmp);

/**
 * @brief Sort `items` ascending with the merge sort algorithm.
 *
 * Merge sort splits the array in half, sorts both halves and merges them
 * back together. It is stable and always runs in O(n log n) time, but it
 * needs a temporary buffer of the same size as the input.
 *
 * @param items The array of elements to sort in place. May be NULL.
 * @param count Number of elements in `items`.
 * @param cmp Comparator deciding the order. Must not be NULL.
 * @return True if the array was sorted, false if the arguments were invalid
 *         or the temporary buffer could not be allocated.
 */
bool algo_merge_sort(void **items, size_t count, cmp_fn cmp);

/**
 * @brief Sort plain integers ascending with the counting sort algorithm.
 *
 * Counting sort counts how often each value occurs and then places the
 * values in order, which makes it an O(n + k) linear time sort. It only
 * works on integers and it needs a bucket per distinct value, so it is a
 * good fit for small ranges of non-negative numbers.
 *
 * @param items The array of integers to sort in place. May be NULL.
 * @param count Number of elements in `items`.
 */
void algo_counting_sort_int(int *items, size_t count);

#endif // SORT_H
