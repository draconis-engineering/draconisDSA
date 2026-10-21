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

#include "sort.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Exchanges two elements of the array.
static void swap_items(void **items, size_t a, size_t b) {
	void *tmp = items[a];
	items[a] = items[b];
	items[b] = tmp;
}

// Rejects calls that cannot do anything useful.
static int sort_arguments_are_valid(void **items, size_t count, cmp_fn cmp) {
	if (items == NULL || cmp == NULL) {
		return 0;
	}
	// Zero or one element is already sorted.
	return count > 1;
}

// ================== //
// -- BUBBLE SORT -- //
// ================== //

/*
 * Bubble sort: walk the array from the back to the front and swap every pair
 * of neighbours that is in the wrong order. Each walk moves the largest
 * remaining element to the end, so the unsorted part gets one element
 * shorter every time.
 */
bool algo_bubble_sort(void **items, size_t count, cmp_fn cmp) {
	if (!sort_arguments_are_valid(items, count, cmp)) {
		return items != NULL && cmp != NULL;
	}

	for (size_t pass = 0; pass + 1 < count; pass++) {
		bool swapped = false;

		for (size_t i = 0; i + 1 < count - pass; i++) {
			if (cmp(items[i], items[i + 1]) > 0) {
				swap_items(items, i, i + 1);
				swapped = true;
			}
		}

		// Nothing moved, so the array is already sorted.
		if (!swapped) {
			break;
		}
	}

	return true;
}

// ===================== //
// -- SELECTION SORT -- //
// ===================== //

/*
 * Selection sort: find the smallest element that is still out of place and
 * swap it into the front of the unsorted part.
 */
bool algo_selection_sort(void **items, size_t count, cmp_fn cmp) {
	if (!sort_arguments_are_valid(items, count, cmp)) {
		return items != NULL && cmp != NULL;
	}

	for (size_t i = 0; i + 1 < count; i++) {
		size_t smallest = i;

		for (size_t j = i + 1; j < count; j++) {
			if (cmp(items[j], items[smallest]) < 0) {
				smallest = j;
			}
		}

		if (smallest != i) {
			swap_items(items, i, smallest);
		}
	}

	return true;
}

// ===================== //
// -- INSERTION SORT -- //
// ===================== //

/*
 * Insertion sort: keep a sorted prefix at the front of the array. Take the
 * next element and move it left until it sits in the right place.
 */
bool algo_insertion_sort(void **items, size_t count, cmp_fn cmp) {
	if (!sort_arguments_are_valid(items, count, cmp)) {
		return items != NULL && cmp != NULL;
	}

	for (size_t i = 1; i < count; i++) {
		void *current = items[i];
		size_t j = i;

		while (j > 0 && cmp(current, items[j - 1]) < 0) {
			items[j] = items[j - 1];
			j--;
		}

		items[j] = current;
	}

	return true;
}

// ================== //
// -- QUICK SORT -- //
// ================== //

/*
 * Partitions the range [low, high] around a pivot and returns the position
 * the pivot ended up in. The pivot is the element in the middle of the
 * range, which keeps already sorted input from degenerating into O(n^2).
 */
static long long partition(void **items, long long low, long long high,
						   cmp_fn cmp) {
	void *pivot = items[low + (high - low) / 2];

	long long left = low - 1;
	long long right = high + 1;

	for (;;) {
		do {
			left++;
		} while (cmp(items[left], pivot) < 0);

		do {
			right--;
		} while (cmp(items[right], pivot) > 0);

		if (left >= right) {
			return right;
		}

		swap_items(items, (size_t)left, (size_t)right);
	}
}

/*
 * Sorts the range [low, high] by partitioning it and sorting both halves.
 */
static void quick_sort_range(void **items, long long low, long long high,
							  cmp_fn cmp) {
	if (low >= high) {
		return;
	}

	long long pivot = partition(items, low, high, cmp);
	quick_sort_range(items, low, pivot, cmp);
	quick_sort_range(items, pivot + 1, high, cmp);
}

/*
 * Quick sort: pick a pivot, split the range around it and sort both halves.
 */
bool algo_quick_sort(void **items, size_t count, cmp_fn cmp) {
	if (!sort_arguments_are_valid(items, count, cmp)) {
		return items != NULL && cmp != NULL;
	}

	quick_sort_range(items, 0, (long long)count - 1, cmp);
	return true;
}

// ================ //
// -- HEAP SORT -- //
// ================ //

// Moves the element at `index` down until both of its children are smaller.
static void heap_sift_down(void **items, size_t count, size_t index,
						   cmp_fn cmp) {
	for (;;) {
		size_t left = 2 * index + 1;
		size_t right = 2 * index + 2;
		size_t largest = index;

		if (left < count && cmp(items[left], items[largest]) > 0) {
			largest = left;
		}
		if (right < count && cmp(items[right], items[largest]) > 0) {
			largest = right;
		}

		if (largest == index) {
			return;
		}

		swap_items(items, index, largest);
		index = largest;
	}
}

/*
 * Heap sort: turn the array into a max-heap, then repeatedly move the
 * largest element to the back of the array.
 */
bool algo_heap_sort(void **items, size_t count, cmp_fn cmp) {
	if (!sort_arguments_are_valid(items, count, cmp)) {
		return items != NULL && cmp != NULL;
	}

	// Build a max-heap. Starting in the middle and working towards the
	// front guarantees that both children of a node are already heaps.
	for (size_t i = count / 2; i > 0; i--) {
		heap_sift_down(items, count, i - 1, cmp);
	}

	// Move the largest element to the end, then repair the heap.
	for (size_t end = count; end > 1; end--) {
		swap_items(items, 0, end - 1);
		heap_sift_down(items, end - 1, 0, cmp);
	}

	return true;
}

// ================== //
// -- MERGE SORT -- //
// ================== //

/*
 * Merges the two sorted ranges [left, middle] and [middle + 1, right] into
 * the temporary buffer, then writes the result back into the array.
 */
static void merge_ranges(void **items, void **temp, size_t left, size_t middle,
						 size_t right, cmp_fn cmp) {
	size_t i = left;
	size_t j = middle + 1;
	size_t k = left;

	while (i <= middle && j <= right) {
		// Taking the left element on a tie is what makes the sort stable.
		if (cmp(items[i], items[j]) <= 0) {
			temp[k++] = items[i++];
		} else {
			temp[k++] = items[j++];
		}
	}

	while (i <= middle) {
		temp[k++] = items[i++];
	}
	while (j <= right) {
		temp[k++] = items[j++];
	}

	for (size_t index = left; index <= right; index++) {
		items[index] = temp[index];
	}
}

/*
 * Merge sort: split the array in half, sort both halves and merge them.
 */
static void merge_sort_range(void **items, void **temp, size_t left,
							 size_t right, cmp_fn cmp) {
	if (left >= right) {
		return;
	}

	size_t middle = left + (right - left) / 2;

	merge_sort_range(items, temp, left, middle, cmp);
	merge_sort_range(items, temp, middle + 1, right, cmp);
	merge_ranges(items, temp, left, middle, right, cmp);
}

/*
 * Merge sort: a stable O(n log n) sort that needs a temporary buffer.
 */
bool algo_merge_sort(void **items, size_t count, cmp_fn cmp) {
	if (!sort_arguments_are_valid(items, count, cmp)) {
		return items != NULL && cmp != NULL;
	}

	void **temp = (void **)malloc(count * sizeof(void *));
	if (temp == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return false;
	}

	merge_sort_range(items, temp, 0, count - 1, cmp);

	free(temp);
	return true;
}

// ==================== //
// -- COUNTING SORT -- //
// ==================== //

/*
 * Counting sort: count how often every value occurs, then place the values
 * in order. The values have to be integers, and the bucket array has to be
 * as large as the range between the smallest and the largest value.
 */
void algo_counting_sort_int(int *items, size_t count) {
	if (items == NULL || count < 2) {
		return;
	}

	int smallest = items[0];
	int largest = items[0];

	for (size_t i = 1; i < count; i++) {
		if (items[i] < smallest) {
			smallest = items[i];
		}
		if (items[i] > largest) {
			largest = items[i];
		}
	}

	// The range has to fit in a size_t, otherwise the bucket array would
	// be impossibly large.
	int64_t range = (int64_t)largest - (int64_t)smallest + 1;
	if (range < 0 ||
		(uint64_t)range > (uint64_t)(size_t)-1 / sizeof(size_t)) {
		fprintf(stderr, "Error: range too large for counting sort\n");
		return;
	}

	size_t bucket_count = (size_t)range;
	size_t *counts = (size_t *)calloc(bucket_count, sizeof(size_t));
	int *output = (int *)malloc(count * sizeof(int));
	if (counts == NULL || output == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		free(counts);
		free(output);
		return;
	}

	for (size_t i = 0; i < count; i++) {
		counts[(size_t)((int64_t)items[i] - smallest)]++;
	}

	// Turn the counts into the ending position of every bucket.
	size_t total = 0;
	for (size_t i = 0; i < bucket_count; i++) {
		total += counts[i];
		counts[i] = total;
	}

	// Walk the array backwards and drop every value into its slot. Going
	// backwards keeps values that are equal in their original order.
	for (size_t i = count; i > 0; i--) {
		int value = items[i - 1];
		size_t bucket = (size_t)((int64_t)value - smallest);
		output[--counts[bucket]] = value;
	}

	for (size_t i = 0; i < count; i++) {
		items[i] = output[i];
	}

	free(output);
	free(counts);
}
