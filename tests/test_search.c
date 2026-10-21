/*
 * Searching Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "algorithms/search.h"

// Orders ints ascending.
static int cmp_int(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x > y) - (x < y);
}

// Builds a sorted array of pointers to ints.
static void build_sorted(void **items, int *values, int count) {
	values[0] = 1;
	values[1] = 3;
	values[2] = 5;
	values[3] = 7;
	values[4] = 9;

	for (int i = 0; i < count; i++) {
		items[i] = &values[i];
	}
}

// A linear search finds an element that is in the array.
TEST(test_algo_linear_search_finds_element) {
	int values[5];
	void *items[5];
	build_sorted(items, values, 5);

	int target = 7;
	ASSERT_EQ_LONG(algo_linear_search(items, 5, &values[2], cmp_int), 2);
	ASSERT_EQ_LONG(algo_linear_search(items, 5, &target, cmp_int), 3);
}

// A linear search reports a missing element.
TEST(test_algo_linear_search_reports_missing_element) {
	int values[5];
	void *items[5];
	build_sorted(items, values, 5);

	int target = 4;
	ASSERT_EQ_LONG(algo_linear_search(items, 5, &target, cmp_int), -1);
}

// A binary search finds an element in a sorted array.
TEST(test_algo_binary_search_finds_element) {
	int values[5];
	void *items[5];
	build_sorted(items, values, 5);

	int target = 7;
	ASSERT_EQ_LONG(algo_binary_search(items, 5, &values[0], cmp_int), 0);
	ASSERT_EQ_LONG(algo_binary_search(items, 5, &values[4], cmp_int), 4);
	ASSERT_EQ_LONG(algo_binary_search(items, 5, &target, cmp_int), 3);
}

// A binary search reports a missing element.
TEST(test_algo_binary_search_reports_missing_element) {
	int values[5];
	void *items[5];
	build_sorted(items, values, 5);

	int target = 4;
	ASSERT_EQ_LONG(algo_binary_search(items, 5, &target, cmp_int), -1);

	// Smaller and larger than every element in the array.
	int smaller = 0;
	int larger = 100;
	ASSERT_EQ_LONG(algo_binary_search(items, 5, &smaller, cmp_int), -1);
	ASSERT_EQ_LONG(algo_binary_search(items, 5, &larger, cmp_int), -1);
}

// Both searches cope with an empty array and with invalid arguments.
TEST(test_algo_searches_handle_edge_cases) {
	int values[5];
	void *items[5];
	build_sorted(items, values, 5);

	int target = 3;

	ASSERT_EQ_LONG(algo_linear_search(items, 0, &target, cmp_int), -1);
	ASSERT_EQ_LONG(algo_binary_search(items, 0, &target, cmp_int), -1);

	ASSERT_EQ_LONG(algo_linear_search(NULL, 5, &target, cmp_int), -1);
	ASSERT_EQ_LONG(algo_linear_search(items, 5, &target, NULL), -1);
	ASSERT_EQ_LONG(algo_binary_search(NULL, 5, &target, cmp_int), -1);
	ASSERT_EQ_LONG(algo_binary_search(items, 5, &target, NULL), -1);
}

// Runs every searching test case.
void run_search_tests(void) {
	test_section("search");

	RUN_TEST(test_algo_linear_search_finds_element);
	RUN_TEST(test_algo_linear_search_reports_missing_element);
	RUN_TEST(test_algo_binary_search_finds_element);
	RUN_TEST(test_algo_binary_search_reports_missing_element);
	RUN_TEST(test_algo_searches_handle_edge_cases);
}
