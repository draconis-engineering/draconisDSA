/*
 * Heap Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "test.h"
#include "tests.h"

#include "heap/heap.h"

// Orders ints ascending, which makes the heap a min-heap.
static int cmp_int(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x > y) - (x < y);
}

// Orders ints descending, which turns the same heap into a max-heap.
static int cmp_int_desc(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x < y) - (x > y);
}

// A heap without a comparator cannot decide on an order.
TEST(test_heap_requires_a_comparator) {
	ASSERT_NULL(create_heap(NULL));
}

// A new heap is empty.
TEST(test_heap_create_is_empty) {
	struct Heap *heap = create_heap(cmp_int);

	ASSERT_NOT_NULL(heap);
	ASSERT_EQ_UINT(heap_size(heap), 0);
	ASSERT(heap_is_empty(heap));
	ASSERT_NULL(heap_peek(heap));
	ASSERT_NULL(heap_pop(heap));

	free_heap(heap);
}

// The smallest element comes out first.
TEST(test_heap_pops_in_ascending_order) {
	struct Heap *heap = create_heap(cmp_int);
	int values[] = {5, 1, 8, 3, 9, 2, 7};

	for (int i = 0; i < 7; i++) {
		ASSERT(heap_push(heap, &values[i]));
	}

	ASSERT_EQ_UINT(heap_size(heap), 7);
	ASSERT_FALSE(heap_is_empty(heap));

	// The positions of the values, sorted from the smallest to the largest.
	int expected[] = {1, 5, 3, 0, 6, 2, 4};
	for (int i = 0; i < 7; i++) {
		ASSERT_PTR_EQ(heap_peek(heap), &values[expected[i]]);
		ASSERT_PTR_EQ(heap_pop(heap), &values[expected[i]]);
	}

	ASSERT(heap_is_empty(heap));
	ASSERT_NULL(heap_pop(heap));

	free_heap(heap);
}

// Flipping the comparator turns the heap into a max-heap.
TEST(test_heap_can_be_a_max_heap) {
	struct Heap *heap = create_heap(cmp_int_desc);
	int values[] = {5, 1, 8, 3, 9, 2, 7};

	for (int i = 0; i < 7; i++) {
		heap_push(heap, &values[i]);
	}

	// The positions of the values, sorted from the largest to the smallest.
	int expected[] = {4, 2, 6, 0, 3, 5, 1};
	for (int i = 0; i < 7; i++) {
		ASSERT_PTR_EQ(heap_pop(heap), &values[expected[i]]);
	}

	free_heap(heap);
}

// Peeking never removes the top element.
TEST(test_heap_peek_keeps_element) {
	struct Heap *heap = create_heap(cmp_int);
	int values[] = {4, 2, 6};

	heap_push(heap, &values[0]);
	heap_push(heap, &values[1]);
	heap_push(heap, &values[2]);

	for (int i = 0; i < 3; i++) {
		ASSERT_PTR_EQ(heap_peek(heap), &values[1]);
	}

	ASSERT_EQ_UINT(heap_size(heap), 3);

	free_heap(heap);
}

// The buffer grows when the heap fills up.
TEST(test_heap_grows_beyond_the_initial_capacity) {
	struct Heap *heap = create_heap(cmp_int);
	int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	for (int i = 0; i < 10; i++) {
		ASSERT(heap_push(heap, &values[i]));
	}

	ASSERT_EQ_UINT(heap_size(heap), 10);
	ASSERT(heap->capacity >= 10);

	// Everything is still in the right order after the growth.
	for (int i = 0; i < 10; i++) {
		ASSERT_PTR_EQ(heap_pop(heap), &values[i]);
	}

	free_heap(heap);
}

// Pushing the same element twice keeps both copies.
TEST(test_heap_keeps_duplicates) {
	struct Heap *heap = create_heap(cmp_int);
	int value = 5;

	ASSERT(heap_push(heap, &value));
	ASSERT(heap_push(heap, &value));

	ASSERT_EQ_UINT(heap_size(heap), 2);
	ASSERT_PTR_EQ(heap_pop(heap), &value);
	ASSERT_PTR_EQ(heap_pop(heap), &value);

	free_heap(heap);
}

// Freeing is safe with a NULL pointer.
TEST(test_heap_free_is_null_safe) {
	free_heap(NULL);
}

// Runs every heap test case.
void run_heap_tests(void) {
	test_section("heap");

	RUN_TEST(test_heap_requires_a_comparator);
	RUN_TEST(test_heap_create_is_empty);
	RUN_TEST(test_heap_pops_in_ascending_order);
	RUN_TEST(test_heap_can_be_a_max_heap);
	RUN_TEST(test_heap_peek_keeps_element);
	RUN_TEST(test_heap_grows_beyond_the_initial_capacity);
	RUN_TEST(test_heap_keeps_duplicates);
	RUN_TEST(test_heap_free_is_null_safe);
}
