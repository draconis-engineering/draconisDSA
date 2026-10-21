/*
 * Queue Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "queue/queue.h"

// A new queue is empty.
TEST(test_queue_create_is_empty) {
	struct Queue *q = create_queue();

	ASSERT_NOT_NULL(q);
	ASSERT_NULL(queue_get(q));
	ASSERT_NULL(queue_peek(q));

	free_queue(q);
}

// A queue is first in, first out.
TEST(test_queue_is_fifo) {
	struct Queue *q = create_queue();
	int values[] = {1, 2, 3};

	queue_add(q, &values[0]);
	queue_add(q, &values[1]);
	queue_add(q, &values[2]);

	ASSERT_PTR_EQ(queue_peek(q), &values[0]);
	ASSERT_PTR_EQ(queue_get(q), &values[0]);
	ASSERT_PTR_EQ(queue_get(q), &values[1]);
	ASSERT_PTR_EQ(queue_get(q), &values[2]);

	free_queue(q);
}

// Peeking never removes the front element.
TEST(test_queue_peek_keeps_element) {
	struct Queue *q = create_queue();
	int value = 7;

	queue_add(q, &value);

	for (int i = 0; i < 3; i++) {
		ASSERT_PTR_EQ(queue_peek(q), &value);
	}

	ASSERT_PTR_EQ(queue_get(q), &value);

	free_queue(q);
}

// Taking from an empty queue returns NULL instead of crashing.
TEST(test_queue_get_on_empty_returns_null) {
	struct Queue *q = create_queue();

	ASSERT_NULL(queue_get(q));
	ASSERT_NULL(queue_peek(q));

	free_queue(q);
}

// Freeing is safe with a NULL pointer.
TEST(test_queue_free_is_null_safe) {
	free_queue(NULL);
}

// Runs every queue test case.
void run_queue_tests(void) {
	test_section("queue");

	RUN_TEST(test_queue_create_is_empty);
	RUN_TEST(test_queue_is_fifo);
	RUN_TEST(test_queue_peek_keeps_element);
	RUN_TEST(test_queue_get_on_empty_returns_null);
	RUN_TEST(test_queue_free_is_null_safe);
}
