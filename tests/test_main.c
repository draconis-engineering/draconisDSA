/*
 * Test Suite Entry Point - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

int main(void) {
	test_begin("DraconisDSA");

	run_array_tests();
	run_sllist_tests();
	run_dllist_tests();
	run_stack_tests();
	run_queue_tests();
	run_hashmap_tests();
	run_hashset_tests();
	run_graph_tests();
	run_trees_tests();
	run_heap_tests();
	run_sort_tests();
	run_search_tests();
	run_traversal_tests();

	return test_end();
}
