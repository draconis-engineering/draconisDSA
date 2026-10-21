/*
 * Doubly Linked List Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "dllist/dllist.h"

// A new list is empty and has no head or tail.
TEST(test_dll_create_is_empty) {
	struct DLList *l = create_dll();

	ASSERT_NOT_NULL(l);
	ASSERT_EQ_INT(dll_size(l), 0);
	ASSERT_EQ_INT(dll_is_empty(l), 1);
	ASSERT_NULL(l->head);
	ASSERT_NULL(l->tail);

	free_dll(l);
}

// Inserting at the front and at the back keeps head, tail and the back
// pointers correct.
TEST(test_dll_insert_front_and_back) {
	struct DLList *l = create_dll();
	int values[] = {1, 2, 3};

	dll_insert_at_back(l, &values[1]);
	ASSERT_EQ_INT(dll_size(l), 1);
	ASSERT_PTR_EQ(l->head->data, &values[1]);
	ASSERT_PTR_EQ(l->tail->data, &values[1]);

	dll_insert_at_front(l, &values[0]);
	ASSERT_EQ_INT(dll_size(l), 2);
	ASSERT_PTR_EQ(l->head->data, &values[0]);
	ASSERT_PTR_EQ(l->head->next->data, &values[1]);
	ASSERT_PTR_EQ(l->head->next->prev, l->head);
	ASSERT_NULL(l->head->prev);

	dll_insert_at_back(l, &values[2]);
	ASSERT_PTR_EQ(l->tail->data, &values[2]);
	ASSERT_PTR_EQ(l->tail->prev->data, &values[1]);
	ASSERT_NULL(l->tail->next);

	free_dll(l);
}

// Inserting by index wires both the next and the prev pointer.
TEST(test_dll_insert_by_index) {
	struct DLList *l = create_dll();
	int values[] = {1, 2, 3, 4};

	dll_insert_at_back(l, &values[0]);
	dll_insert_at_back(l, &values[1]);
	dll_insert_at_back(l, &values[2]);

	dll_insert(l, &values[3], 1);
	ASSERT_EQ_INT(dll_size(l), 4);

	struct DLLNode *middle = l->head->next;
	ASSERT_PTR_EQ(middle->data, &values[3]);
	ASSERT_PTR_EQ(middle->prev, l->head);
	ASSERT_PTR_EQ(middle->next, l->head->next->next);
	ASSERT_PTR_EQ(middle->next->prev, middle);

	// Index -1 is the documented shortcut for "insert at the back".
	dll_insert(l, &values[0], -1);
	ASSERT_EQ_INT(dll_size(l), 5);
	ASSERT_PTR_EQ(l->tail->data, &values[0]);

	free_dll(l);
}

// An out-of-range insert is rejected and leaves the list untouched.
TEST(test_dll_insert_out_of_range_is_ignored) {
	struct DLList *l = create_dll();
	int values[] = {1, 2};

	dll_insert_at_back(l, &values[0]);

	dll_insert(l, &values[1], 9);
	dll_insert(l, &values[1], -2);

	ASSERT_EQ_INT(dll_size(l), 1);
	ASSERT_PTR_EQ(l->head->data, &values[0]);

	free_dll(l);
}

// The index lookup compares by pointer identity.
TEST(test_dll_get_idx) {
	struct DLList *l = create_dll();
	int values[] = {1, 2, 3};
	int missing = 99;

	dll_insert_at_back(l, &values[0]);
	dll_insert_at_back(l, &values[1]);
	dll_insert_at_back(l, &values[2]);

	ASSERT_EQ_INT(dll_get_idx(l, &values[0]), 0);
	ASSERT_EQ_INT(dll_get_idx(l, &values[2]), 2);
	ASSERT_EQ_INT(dll_get_idx(l, &missing), -1);

	free_dll(l);
}

// Removing from the middle relinks both neighbours.
TEST(test_dll_remove_from_middle) {
	struct DLList *l = create_dll();
	int values[] = {1, 2, 3, 4};

	for (int i = 0; i < 4; i++) {
		dll_insert_at_back(l, &values[i]);
	}

	dll_remove(l, 1);
	ASSERT_EQ_INT(dll_size(l), 3);

	struct DLLNode *second = l->head->next;
	ASSERT_PTR_EQ(second->data, &values[2]);
	ASSERT_PTR_EQ(second->prev, l->head);
	ASSERT_PTR_EQ(second->prev->next, second);

	free_dll(l);
}

// Removing the head and the tail keeps the remaining links intact.
TEST(test_dll_remove_head_and_tail) {
	struct DLList *l = create_dll();
	int values[] = {1, 2, 3};

	for (int i = 0; i < 3; i++) {
		dll_insert_at_back(l, &values[i]);
	}

	dll_remove(l, 2);
	ASSERT_EQ_INT(dll_size(l), 2);
	ASSERT_PTR_EQ(l->tail->data, &values[1]);
	ASSERT_NULL(l->tail->next);
	ASSERT_PTR_EQ(l->tail->prev, l->head);

	dll_remove(l, 0);
	ASSERT_EQ_INT(dll_size(l), 1);
	ASSERT_PTR_EQ(l->head->data, &values[1]);
	ASSERT_PTR_EQ(l->tail->data, &values[1]);
	ASSERT_NULL(l->head->prev);
	ASSERT_NULL(l->head->next);

	free_dll(l);
}

// Removing the only element clears both head and tail.
TEST(test_dll_remove_last_element) {
	struct DLList *l = create_dll();
	int value = 1;

	dll_insert_at_back(l, &value);
	dll_remove(l, 0);

	ASSERT_EQ_INT(dll_size(l), 0);
	ASSERT_NULL(l->head);
	ASSERT_NULL(l->tail);

	free_dll(l);
}

// An out-of-range removal is rejected and leaves the list untouched.
TEST(test_dll_remove_out_of_range_is_ignored) {
	struct DLList *l = create_dll();
	int values[] = {1, 2};

	dll_insert_at_back(l, &values[0]);
	dll_insert_at_back(l, &values[1]);

	dll_remove(l, 5);
	dll_remove(l, -1);

	ASSERT_EQ_INT(dll_size(l), 2);

	free_dll(l);
}

// Freeing is safe with a NULL pointer.
TEST(test_dll_free_is_null_safe) {
	free_dll(NULL);
}

// Runs every doubly linked list test case.
void run_dllist_tests(void) {
	test_section("dllist");

	RUN_TEST(test_dll_create_is_empty);
	RUN_TEST(test_dll_insert_front_and_back);
	RUN_TEST(test_dll_insert_by_index);
	RUN_TEST(test_dll_insert_out_of_range_is_ignored);
	RUN_TEST(test_dll_get_idx);
	RUN_TEST(test_dll_remove_from_middle);
	RUN_TEST(test_dll_remove_head_and_tail);
	RUN_TEST(test_dll_remove_last_element);
	RUN_TEST(test_dll_remove_out_of_range_is_ignored);
	RUN_TEST(test_dll_free_is_null_safe);
}
