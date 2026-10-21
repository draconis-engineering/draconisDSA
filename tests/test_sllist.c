/*
 * Singly Linked List Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "sllist/sllist.h"

// A new list is empty and has no head or tail.
TEST(test_sll_create_is_empty) {
	struct SLList *l = create_sll();

	ASSERT_NOT_NULL(l);
	ASSERT_EQ_INT(sll_size(l), 0);
	ASSERT_EQ_INT(sll_is_empty(l), 1);
	ASSERT_NULL(l->head);
	ASSERT_NULL(l->tail);

	free_sll(l);
}

// Inserting at the front and at the back keeps head and tail correct.
TEST(test_sll_insert_front_and_back) {
	struct SLList *l = create_sll();
	int values[] = {1, 2, 3};

	sll_insert_at_back(l, &values[1]);
	ASSERT_EQ_INT(sll_size(l), 1);
	ASSERT_PTR_EQ(l->head->data, &values[1]);
	ASSERT_PTR_EQ(l->tail->data, &values[1]);

	sll_insert_at_front(l, &values[0]);
	ASSERT_EQ_INT(sll_size(l), 2);
	ASSERT_PTR_EQ(l->head->data, &values[0]);
	ASSERT_PTR_EQ(l->tail->data, &values[1]);

	sll_insert_at_back(l, &values[2]);
	ASSERT_EQ_INT(sll_size(l), 3);
	ASSERT_PTR_EQ(l->tail->data, &values[2]);

	// head -> 1 -> 2 -> 3 -> tail
	ASSERT_PTR_EQ(l->head->next->data, &values[1]);
	ASSERT_PTR_EQ(l->head->next->next->data, &values[2]);
	ASSERT_NULL(l->tail->next);

	free_sll(l);
}

// Inserting by index places the element before the one currently there.
TEST(test_sll_insert_by_index) {
	struct SLList *l = create_sll();
	int values[] = {1, 2, 3, 4};

	sll_insert_at_back(l, &values[0]);
	sll_insert_at_back(l, &values[1]);
	sll_insert_at_back(l, &values[2]);

	sll_insert(l, &values[3], 1);
	ASSERT_EQ_INT(sll_size(l), 4);
	ASSERT_PTR_EQ(l->head->next->data, &values[3]);
	ASSERT_PTR_EQ(l->head->next->next->data, &values[1]);

	// Inserting at the size appends.
	sll_insert(l, &values[0], sll_size(l));
	ASSERT_EQ_INT(sll_size(l), 5);
	ASSERT_PTR_EQ(l->tail->data, &values[0]);

	// Index -1 is the documented shortcut for "insert at the back".
	sll_insert(l, &values[1], -1);
	ASSERT_EQ_INT(sll_size(l), 6);
	ASSERT_PTR_EQ(l->tail->data, &values[1]);

	free_sll(l);
}

// An out-of-range insert is rejected and leaves the list untouched.
TEST(test_sll_insert_out_of_range_is_ignored) {
	struct SLList *l = create_sll();
	int values[] = {1, 2};

	sll_insert_at_back(l, &values[0]);

	sll_insert(l, &values[1], 9);
	sll_insert(l, &values[1], -2);

	ASSERT_EQ_INT(sll_size(l), 1);
	ASSERT_PTR_EQ(l->head->data, &values[0]);

	free_sll(l);
}

// The index lookup compares by pointer identity.
TEST(test_sll_get_idx) {
	struct SLList *l = create_sll();
	int values[] = {1, 2, 3};
	int missing = 99;

	sll_insert_at_back(l, &values[0]);
	sll_insert_at_back(l, &values[1]);
	sll_insert_at_back(l, &values[2]);

	ASSERT_EQ_INT(sll_get_idx(l, &values[0]), 0);
	ASSERT_EQ_INT(sll_get_idx(l, &values[2]), 2);
	ASSERT_EQ_INT(sll_get_idx(l, &missing), -1);

	free_sll(l);
}

// Removing from the front, the middle and the back relinks the list.
TEST(test_sll_remove_relinks) {
	struct SLList *l = create_sll();
	int values[] = {1, 2, 3, 4};

	for (int i = 0; i < 4; i++) {
		sll_insert_at_back(l, &values[i]);
	}

	// Remove from the middle: v1 disappears and the rest close up.
	sll_remove(l, 1);
	ASSERT_EQ_INT(sll_size(l), 3);
	ASSERT_PTR_EQ(l->head->data, &values[0]);
	ASSERT_PTR_EQ(l->head->next->data, &values[2]);
	ASSERT_PTR_EQ(l->tail->data, &values[3]);

	// Remove the tail: the tail pointer has to move back.
	sll_remove(l, 2);
	ASSERT_EQ_INT(sll_size(l), 2);
	ASSERT_PTR_EQ(l->tail->data, &values[2]);
	ASSERT_NULL(l->tail->next);

	// Remove the head: the tail has to survive.
	sll_remove(l, 0);
	ASSERT_EQ_INT(sll_size(l), 1);
	ASSERT_PTR_EQ(l->head->data, &values[2]);
	ASSERT_PTR_EQ(l->tail->data, &values[2]);
	ASSERT_NULL(l->head->next);

	free_sll(l);
}

// Removing the only element clears both head and tail.
TEST(test_sll_remove_last_element) {
	struct SLList *l = create_sll();
	int value = 1;

	sll_insert_at_back(l, &value);
	sll_remove(l, 0);

	ASSERT_EQ_INT(sll_size(l), 0);
	ASSERT_EQ_INT(sll_is_empty(l), 1);
	ASSERT_NULL(l->head);
	ASSERT_NULL(l->tail);

	free_sll(l);
}

// An out-of-range removal is rejected and leaves the list untouched.
TEST(test_sll_remove_out_of_range_is_ignored) {
	struct SLList *l = create_sll();
	int values[] = {1, 2};

	sll_insert_at_back(l, &values[0]);
	sll_insert_at_back(l, &values[1]);

	sll_remove(l, 5);
	sll_remove(l, -1);

	ASSERT_EQ_INT(sll_size(l), 2);

	free_sll(l);
}

// Freeing is safe with a NULL pointer.
TEST(test_sll_free_is_null_safe) {
	free_sll(NULL);
}

// Runs every singly linked list test case.
void run_sllist_tests(void) {
	test_section("sllist");

	RUN_TEST(test_sll_create_is_empty);
	RUN_TEST(test_sll_insert_front_and_back);
	RUN_TEST(test_sll_insert_by_index);
	RUN_TEST(test_sll_insert_out_of_range_is_ignored);
	RUN_TEST(test_sll_get_idx);
	RUN_TEST(test_sll_remove_relinks);
	RUN_TEST(test_sll_remove_last_element);
	RUN_TEST(test_sll_remove_out_of_range_is_ignored);
	RUN_TEST(test_sll_free_is_null_safe);
}
