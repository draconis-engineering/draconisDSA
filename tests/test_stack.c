/*
 * Stack Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "stack/stack.h"

// A new stack is empty.
TEST(test_stack_create_is_empty) {
	struct Stack *s = create_stack();

	ASSERT_NOT_NULL(s);
	ASSERT_NULL(stack_pop(s));
	ASSERT_NULL(stack_peek(s));

	free_stack(s);
}

// A stack is last in, first out.
TEST(test_stack_is_lifo) {
	struct Stack *s = create_stack();
	int values[] = {1, 2, 3};

	stack_push(s, &values[0]);
	stack_push(s, &values[1]);
	stack_push(s, &values[2]);

	ASSERT_PTR_EQ(stack_peek(s), &values[2]);
	ASSERT_PTR_EQ(stack_pop(s), &values[2]);
	ASSERT_PTR_EQ(stack_pop(s), &values[1]);
	ASSERT_PTR_EQ(stack_pop(s), &values[0]);

	free_stack(s);
}

// Peeking never removes the top element.
TEST(test_stack_peek_keeps_element) {
	struct Stack *s = create_stack();
	int value = 7;

	stack_push(s, &value);

	for (int i = 0; i < 3; i++) {
		ASSERT_PTR_EQ(stack_peek(s), &value);
	}

	ASSERT_PTR_EQ(stack_pop(s), &value);

	free_stack(s);
}

// Popping an empty stack returns NULL instead of crashing.
TEST(test_stack_pop_on_empty_returns_null) {
	struct Stack *s = create_stack();

	ASSERT_NULL(stack_pop(s));
	ASSERT_NULL(stack_peek(s));

	free_stack(s);
}

// Freeing is safe with a NULL pointer.
TEST(test_stack_free_is_null_safe) {
	free_stack(NULL);
}

// Runs every stack test case.
void run_stack_tests(void) {
	test_section("stack");

	RUN_TEST(test_stack_create_is_empty);
	RUN_TEST(test_stack_is_lifo);
	RUN_TEST(test_stack_peek_keeps_element);
	RUN_TEST(test_stack_pop_on_empty_returns_null);
	RUN_TEST(test_stack_free_is_null_safe);
}
