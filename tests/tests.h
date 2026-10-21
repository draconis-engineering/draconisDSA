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

#ifndef TESTS_H
#define TESTS_H

/**
 * @brief Run every test case belonging to one module.
 *
 * Each module keeps its test cases in `tests/test_<module>.c` and exposes
 * one entry point here. Adding a module means adding one file and one
 * declaration - the runner below is the only place that has to change.
 */
void run_array_tests(void);
void run_sllist_tests(void);
void run_dllist_tests(void);
void run_stack_tests(void);
void run_queue_tests(void);
void run_hashmap_tests(void);
void run_hashset_tests(void);
void run_graph_tests(void);
void run_trees_tests(void);
void run_heap_tests(void);
void run_sort_tests(void);
void run_search_tests(void);
void run_traversal_tests(void);

#endif // TESTS_H
