/*
 * Array Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "array/array.h"

// Comparator that orders plain ints ascending.
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

// A freshly created array is empty and has room for a few elements.
TEST(test_array_create_is_empty) {
	struct Array *a = create_array();

	ASSERT_NOT_NULL(a);
	ASSERT_EQ_INT(arr_size(a), 0);
	ASSERT_EQ_INT(arr_is_empty(a), 1);
	ASSERT(a->capacity >= 4);

	free_array(a);
}

// Pushing elements grows the size and keeps them in insertion order.
TEST(test_array_push_keeps_order) {
	struct Array *a = create_array();
	int values[] = {10, 20, 30, 40, 50};

	for (int i = 0; i < 5; i++) {
		arr_push(a, &values[i]);
	}

	ASSERT_EQ_INT(arr_size(a), 5);
	ASSERT_EQ_INT(arr_is_empty(a), 0);
	for (int i = 0; i < 5; i++) {
		ASSERT_PTR_EQ(arr_get(a, i), &values[i]);
	}

	free_array(a);
}

// The buffer doubles when it fills up, so appends stay amortized O(1).
TEST(test_array_push_grows_capacity) {
	struct Array *a = create_array();
	int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	unsigned int initial_capacity = a->capacity;

	for (int i = 0; i < 9; i++) {
		arr_push(a, &values[i]);
	}

	ASSERT_EQ_INT(arr_size(a), 9);
	ASSERT(a->capacity > initial_capacity);
	ASSERT(a->capacity >= 9);

	free_array(a);
}

// Inserting shifts the elements on the right one slot to the right.
TEST(test_array_insert_shifts_right) {
	struct Array *a = create_array();
	int values[] = {1, 2, 3, 4};

	arr_push(a, &values[0]);
	arr_push(a, &values[1]);
	arr_push(a, &values[2]);

	arr_insert(a, &values[3], 0);
	ASSERT_EQ_INT(arr_size(a), 4);
	ASSERT_PTR_EQ(arr_get(a, 0), &values[3]);
	ASSERT_PTR_EQ(arr_get(a, 1), &values[0]);
	ASSERT_PTR_EQ(arr_get(a, 2), &values[1]);
	ASSERT_PTR_EQ(arr_get(a, 3), &values[2]);

	arr_insert(a, &values[1], 2);
	ASSERT_EQ_INT(arr_size(a), 5);
	ASSERT_PTR_EQ(arr_get(a, 2), &values[1]);

	// Inserting at the end is allowed and appends.
	arr_insert(a, &values[0], arr_size(a));
	ASSERT_EQ_INT(arr_size(a), 6);
	ASSERT_PTR_EQ(arr_get(a, 5), &values[0]);

	free_array(a);
}

// An out-of-range insert is rejected and leaves the array untouched.
TEST(test_array_insert_out_of_range_is_ignored) {
	struct Array *a = create_array();
	int values[] = {1, 2};

	arr_push(a, &values[0]);

	arr_insert(a, &values[1], 5);
	arr_insert(a, &values[1], -1);

	ASSERT_EQ_INT(arr_size(a), 1);
	ASSERT_PTR_EQ(arr_get(a, 0), &values[0]);

	free_array(a);
}

// Reading and writing by index works, and out-of-range access is rejected.
TEST(test_array_get_and_set) {
	struct Array *a = create_array();
	int values[] = {1, 2, 3};

	arr_push(a, &values[0]);
	arr_push(a, &values[1]);

	arr_set(a, 0, &values[2]);
	ASSERT_PTR_EQ(arr_get(a, 0), &values[2]);

	ASSERT_NULL(arr_get(a, 7));
	ASSERT_NULL(arr_get(a, -1));

	free_array(a);
}

// Removing shifts the later elements one slot to the left.
TEST(test_array_remove_shifts_left) {
	struct Array *a = create_array();
	int values[] = {1, 2, 3, 4};

	for (int i = 0; i < 4; i++) {
		arr_push(a, &values[i]);
	}

	arr_remove(a, 1);
	ASSERT_EQ_INT(arr_size(a), 3);
	ASSERT_PTR_EQ(arr_get(a, 0), &values[0]);
	ASSERT_PTR_EQ(arr_get(a, 1), &values[2]);
	ASSERT_PTR_EQ(arr_get(a, 2), &values[3]);

	// Removing out of range changes nothing.
	arr_remove(a, 9);
	ASSERT_EQ_INT(arr_size(a), 3);

	free_array(a);
}

// Popping returns the last element and shrinks the array.
TEST(test_array_pop_returns_last) {
	struct Array *a = create_array();
	int values[] = {1, 2, 3};

	for (int i = 0; i < 3; i++) {
		arr_push(a, &values[i]);
	}

	void *popped = arr_pop(a);
	ASSERT_PTR_EQ(popped, &values[2]);
	ASSERT_EQ_INT(arr_size(a), 2);
	ASSERT_PTR_EQ(arr_get(a, 1), &values[1]);

	free_array(a);
}

// Popping an empty array returns NULL instead of crashing.
TEST(test_array_pop_on_empty_returns_null) {
	struct Array *a = create_array();

	ASSERT_NULL(arr_pop(a));
	ASSERT_EQ_INT(arr_size(a), 0);

	free_array(a);
}

// Freeing is safe with a NULL pointer.
TEST(test_array_free_is_null_safe) {
	free_array(NULL);
}

// The merge sort orders the array ascending.
TEST(test_array_merge_sort_sorts) {
	struct Array *a = create_array();
	int values[] = {5, 3, 9, 1, 7, 2};

	for (int i = 0; i < 6; i++) {
		arr_push(a, &values[i]);
	}

	struct Array *result = arr_merge_sort(a, cmp_int);
	ASSERT_PTR_EQ(result, a);

	int expected[] = {1, 2, 3, 5, 7, 9};
	for (int i = 0; i < 6; i++) {
		ASSERT_EQ_INT(*(int *)arr_get(a, i), expected[i]);
	}

	free_array(a);
}

// Equal keys keep their original relative order, which is what makes the
// merge sort stable.
TEST(test_array_merge_sort_is_stable) {
	struct Array *a = create_array();
	struct SortItem items[] = {
		{2, 0}, {1, 1}, {2, 2}, {1, 3}, {2, 4}, {1, 5},
	};

	for (int i = 0; i < 6; i++) {
		arr_push(a, &items[i]);
	}

	arr_merge_sort(a, cmp_key_only);

	// The keys come out sorted...
	for (int i = 1; i < 6; i++) {
		struct SortItem *previous = (struct SortItem *)arr_get(a, i - 1);
		struct SortItem *current = (struct SortItem *)arr_get(a, i);
		ASSERT(previous->key <= current->key);
	}

	// ...and the sequence numbers of equal keys stay in ascending order.
	for (int i = 1; i < 6; i++) {
		struct SortItem *previous = (struct SortItem *)arr_get(a, i - 1);
		struct SortItem *current = (struct SortItem *)arr_get(a, i);
		if (previous->key == current->key) {
			ASSERT(previous->seq < current->seq);
		}
	}

	free_array(a);
}

// Sorting zero or one element is a no-op and still reports success.
TEST(test_array_merge_sort_handles_tiny_inputs) {
	struct Array *a = create_array();
	int value = 42;

	arr_push(a, &value);

	ASSERT_PTR_EQ(arr_merge_sort(a, cmp_int), a);
	ASSERT_PTR_EQ(arr_get(a, 0), &value);

	free_array(a);
}

// Runs every array test case.
void run_array_tests(void) {
	test_section("array");

	RUN_TEST(test_array_create_is_empty);
	RUN_TEST(test_array_push_keeps_order);
	RUN_TEST(test_array_push_grows_capacity);
	RUN_TEST(test_array_insert_shifts_right);
	RUN_TEST(test_array_insert_out_of_range_is_ignored);
	RUN_TEST(test_array_get_and_set);
	RUN_TEST(test_array_remove_shifts_left);
	RUN_TEST(test_array_pop_returns_last);
	RUN_TEST(test_array_pop_on_empty_returns_null);
	RUN_TEST(test_array_free_is_null_safe);
	RUN_TEST(test_array_merge_sort_sorts);
	RUN_TEST(test_array_merge_sort_is_stable);
	RUN_TEST(test_array_merge_sort_handles_tiny_inputs);
}
