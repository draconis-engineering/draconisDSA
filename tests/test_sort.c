/*
 * Sorting Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "algorithms/sort.h"

// Orders ints ascending.
static int cmp_int(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x > y) - (x < y);
}

// An item with a key and a sequence number, used to prove that a sort is
// stable: equal keys must keep their original relative order.
struct SortItem {
	int key;
	int seq;
};

// Comparator that only looks at the key, so equal keys are "equivalent".
static int cmp_key_only(const void *a, const void *b) {
	int x = ((const struct SortItem *)a)->key;
	int y = ((const struct SortItem *)b)->key;
	return (x > y) - (x < y);
}

// The signature every sorting algorithm in this library shares.
typedef bool (*sort_fn)(void **, size_t, cmp_fn);

// Sorts a copy of `input` and checks that the result matches `expected`.
static void check_sort(sort_fn sort, int *input, int *expected, size_t count) {
	void *items[32];

	for (size_t i = 0; i < count; i++) {
		items[i] = &input[i];
	}

	ASSERT(sort(items, count, cmp_int));

	for (size_t i = 0; i < count; i++) {
		ASSERT_EQ_INT(*(int *)items[i], expected[i]);
	}
}

// The input every sort is checked with.
static void fill_input(int *input) {
	input[0] = 5;
	input[1] = 3;
	input[2] = 9;
	input[3] = 1;
	input[4] = 7;
	input[5] = 2;
	input[6] = 8;
	input[7] = 4;
	input[8] = 6;
	input[9] = 0;
}

// The expected result of sorting that input.
static void fill_expected(int *expected) {
	expected[0] = 0;
	expected[1] = 1;
	expected[2] = 2;
	expected[3] = 3;
	expected[4] = 4;
	expected[5] = 5;
	expected[6] = 6;
	expected[7] = 7;
	expected[8] = 8;
	expected[9] = 9;
}

// The bubble sort puts the elements in order.
TEST(test_algo_bubble_sort) {
	int input[10];
	int expected[10];
	fill_input(input);
	fill_expected(expected);

	check_sort(algo_bubble_sort, input, expected, 10);
}

// The selection sort puts the elements in order.
TEST(test_algo_selection_sort) {
	int input[10];
	int expected[10];
	fill_input(input);
	fill_expected(expected);

	check_sort(algo_selection_sort, input, expected, 10);
}

// The insertion sort puts the elements in order.
TEST(test_algo_insertion_sort) {
	int input[10];
	int expected[10];
	fill_input(input);
	fill_expected(expected);

	check_sort(algo_insertion_sort, input, expected, 10);
}

// The quick sort puts the elements in order.
TEST(test_algo_quick_sort) {
	int input[10];
	int expected[10];
	fill_input(input);
	fill_expected(expected);

	check_sort(algo_quick_sort, input, expected, 10);
}

// The heap sort puts the elements in order.
TEST(test_algo_heap_sort) {
	int input[10];
	int expected[10];
	fill_input(input);
	fill_expected(expected);

	check_sort(algo_heap_sort, input, expected, 10);
}

// The merge sort puts the elements in order.
TEST(test_algo_merge_sort) {
	int input[10];
	int expected[10];
	fill_input(input);
	fill_expected(expected);

	check_sort(algo_merge_sort, input, expected, 10);
}

// Every sort leaves an already sorted array alone.
TEST(test_algo_sorts_handle_sorted_input) {
	int input[5] = {1, 2, 3, 4, 5};
	int expected[5] = {1, 2, 3, 4, 5};

	check_sort(algo_bubble_sort, input, expected, 5);
	check_sort(algo_selection_sort, input, expected, 5);
	check_sort(algo_insertion_sort, input, expected, 5);
	check_sort(algo_quick_sort, input, expected, 5);
	check_sort(algo_heap_sort, input, expected, 5);
	check_sort(algo_merge_sort, input, expected, 5);
}

// Every sort leaves an array that is sorted backwards alone.
TEST(test_algo_sorts_handle_reversed_input) {
	int input[5] = {5, 4, 3, 2, 1};
	int expected[5] = {1, 2, 3, 4, 5};

	check_sort(algo_bubble_sort, input, expected, 5);
	check_sort(algo_selection_sort, input, expected, 5);
	check_sort(algo_insertion_sort, input, expected, 5);
	check_sort(algo_quick_sort, input, expected, 5);
	check_sort(algo_heap_sort, input, expected, 5);
	check_sort(algo_merge_sort, input, expected, 5);
}

// Zero and one element are already sorted.
TEST(test_algo_sorts_handle_tiny_inputs) {
	int value = 42;
	void *items[1] = {&value};

	ASSERT(algo_bubble_sort(items, 0, cmp_int));
	ASSERT(algo_selection_sort(items, 0, cmp_int));
	ASSERT(algo_insertion_sort(items, 0, cmp_int));
	ASSERT(algo_quick_sort(items, 0, cmp_int));
	ASSERT(algo_heap_sort(items, 0, cmp_int));
	ASSERT(algo_merge_sort(items, 0, cmp_int));

	ASSERT(algo_bubble_sort(items, 1, cmp_int));
	ASSERT(algo_selection_sort(items, 1, cmp_int));
	ASSERT(algo_insertion_sort(items, 1, cmp_int));
	ASSERT(algo_quick_sort(items, 1, cmp_int));
	ASSERT(algo_heap_sort(items, 1, cmp_int));
	ASSERT(algo_merge_sort(items, 1, cmp_int));

	ASSERT_EQ_INT(*(int *)items[0], 42);
}

// A sort without a comparator or without an array is rejected.
TEST(test_algo_sorts_reject_invalid_arguments) {
	int value = 1;
	void *items[1] = {&value};

	ASSERT_FALSE(algo_bubble_sort(NULL, 4, cmp_int));
	ASSERT_FALSE(algo_bubble_sort(items, 4, NULL));
	ASSERT_FALSE(algo_selection_sort(NULL, 4, cmp_int));
	ASSERT_FALSE(algo_insertion_sort(NULL, 4, cmp_int));
	ASSERT_FALSE(algo_quick_sort(NULL, 4, cmp_int));
	ASSERT_FALSE(algo_heap_sort(NULL, 4, cmp_int));
	ASSERT_FALSE(algo_merge_sort(NULL, 4, cmp_int));
}

// Equal keys keep their original relative order, which is what makes the
// merge sort stable.
TEST(test_algo_merge_sort_is_stable) {
	struct SortItem items[] = {
		{2, 0}, {1, 1}, {2, 2}, {1, 3}, {2, 4}, {1, 5},
	};
	void *pointers[6];

	for (int i = 0; i < 6; i++) {
		pointers[i] = &items[i];
	}

	ASSERT(algo_merge_sort(pointers, 6, cmp_key_only));

	// The keys come out sorted...
	for (int i = 1; i < 6; i++) {
		struct SortItem *previous = (struct SortItem *)pointers[i - 1];
		struct SortItem *current = (struct SortItem *)pointers[i];
		ASSERT(previous->key <= current->key);
	}

	// ...and the sequence numbers of equal keys stay in ascending order.
	for (int i = 1; i < 6; i++) {
		struct SortItem *previous = (struct SortItem *)pointers[i - 1];
		struct SortItem *current = (struct SortItem *)pointers[i];
		if (previous->key == current->key) {
			ASSERT(previous->seq < current->seq);
		}
	}
}

// The counting sort orders plain integers, including negative ones.
TEST(test_algo_counting_sort_int) {
	int values[] = {4, -2, 7, 0, -5, 3};
	int expected[] = {-5, -2, 0, 3, 4, 7};

	algo_counting_sort_int(values, 6);

	for (int i = 0; i < 6; i++) {
		ASSERT_EQ_INT(values[i], expected[i]);
	}
}

// The counting sort leaves an already sorted array alone.
TEST(test_algo_counting_sort_int_handles_tiny_inputs) {
	int values[] = {1, 2, 3};

	algo_counting_sort_int(values, 3);
	ASSERT_EQ_INT(values[0], 1);
	ASSERT_EQ_INT(values[1], 2);
	ASSERT_EQ_INT(values[2], 3);

	algo_counting_sort_int(values, 0);
	algo_counting_sort_int(NULL, 3);
}

// Runs every sorting test case.
void run_sort_tests(void) {
	test_section("sort");

	RUN_TEST(test_algo_bubble_sort);
	RUN_TEST(test_algo_selection_sort);
	RUN_TEST(test_algo_insertion_sort);
	RUN_TEST(test_algo_quick_sort);
	RUN_TEST(test_algo_heap_sort);
	RUN_TEST(test_algo_merge_sort);
	RUN_TEST(test_algo_sorts_handle_sorted_input);
	RUN_TEST(test_algo_sorts_handle_reversed_input);
	RUN_TEST(test_algo_sorts_handle_tiny_inputs);
	RUN_TEST(test_algo_sorts_reject_invalid_arguments);
	RUN_TEST(test_algo_merge_sort_is_stable);
	RUN_TEST(test_algo_counting_sort_int);
	RUN_TEST(test_algo_counting_sort_int_handles_tiny_inputs);
}
