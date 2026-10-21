/*
 * Hash Set Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "hashset/hashset.h"

// A new set starts out empty.
TEST(test_hashset_create_is_empty) {
	struct HashSet *set = create_hashset();

	ASSERT_NOT_NULL(set);
	ASSERT_EQ_UINT(set->size, 0);
	ASSERT_FALSE(set_contains(set, "draconis"));

	free_hashset(set);
}

// Added values can be found again.
TEST(test_hashset_add_and_contains) {
	struct HashSet *set = create_hashset();

	ASSERT(set_add(set, "a"));
	ASSERT(set_add(set, "b"));

	ASSERT(set_contains(set, "a"));
	ASSERT(set_contains(set, "b"));
	ASSERT_FALSE(set_contains(set, "c"));
	ASSERT_EQ_UINT(set->size, 2);

	free_hashset(set);
}

// A set never stores the same value twice.
TEST(test_hashset_rejects_duplicates) {
	struct HashSet *set = create_hashset();

	ASSERT(set_add(set, "a"));
	ASSERT_FALSE(set_add(set, "a"));
	ASSERT_EQ_UINT(set->size, 1);

	free_hashset(set);
}

// Removing a value makes it disappear, and removing it twice fails.
TEST(test_hashset_remove) {
	struct HashSet *set = create_hashset();

	set_add(set, "a");
	set_add(set, "b");

	ASSERT(set_remove(set, "a"));
	ASSERT_FALSE(set_contains(set, "a"));
	ASSERT_EQ_UINT(set->size, 1);

	ASSERT_FALSE(set_remove(set, "a"));
	ASSERT(set_contains(set, "b"));

	free_hashset(set);
}

// The set keeps its own copy of the value, so the caller's buffer can change.
TEST(test_hashset_owns_the_value) {
	struct HashSet *set = create_hashset();
	char value[] = "mutable";

	ASSERT(set_add(set, value));

	value[0] = 'M';

	ASSERT(set_contains(set, "mutable"));
	ASSERT_FALSE(set_contains(set, "Mutable"));

	free_hashset(set);
}

// The hash always lands inside the bucket array.
TEST(test_hashset_hash_stays_in_range) {
	struct HashSet *set = create_hashset();

	ASSERT(set_hash(set, "draconis") < set->capacity);
	ASSERT(set_hash(set, "") < set->capacity);
	ASSERT_EQ_UINT(set_hash(set, "draconis"), set_hash(set, "draconis"));

	free_hashset(set);
}

// NULL arguments are rejected instead of crashing.
TEST(test_hashset_handles_null_arguments) {
	struct HashSet *set = create_hashset();

	ASSERT_FALSE(set_add(set, NULL));
	ASSERT_FALSE(set_add(NULL, "a"));
	ASSERT_FALSE(set_contains(set, NULL));
	ASSERT_FALSE(set_contains(NULL, "a"));
	ASSERT_FALSE(set_remove(set, NULL));
	ASSERT_FALSE(set_remove(NULL, "a"));

	free_hashset(set);
}

// Freeing is safe with a NULL pointer.
TEST(test_hashset_free_is_null_safe) {
	free_hashset(NULL);
}

// Runs every hash set test case.
void run_hashset_tests(void) {
	test_section("hashset");

	RUN_TEST(test_hashset_create_is_empty);
	RUN_TEST(test_hashset_add_and_contains);
	RUN_TEST(test_hashset_rejects_duplicates);
	RUN_TEST(test_hashset_remove);
	RUN_TEST(test_hashset_owns_the_value);
	RUN_TEST(test_hashset_hash_stays_in_range);
	RUN_TEST(test_hashset_handles_null_arguments);
	RUN_TEST(test_hashset_free_is_null_safe);
}
