/*
 * Hash Map Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "hashmap/hashmap.h"

// A new map starts out empty.
TEST(test_hashmap_create_is_empty) {
	struct HashMap *map = create_hashmap();
	int found = 1;

	ASSERT_NOT_NULL(map);
	ASSERT_EQ_INT(map_get(map, "draconis", &found), -1);
	ASSERT_EQ_INT(found, 0);

	free_hashmap(map);
}

// A stored value can be read back, and a missing key is reported as such.
TEST(test_hashmap_insert_and_get) {
	struct HashMap *map = create_hashmap();
	int found = 0;

	map_insert(map, "draconis", 1);
	map_insert(map, "github", 2);

	ASSERT_EQ_INT(map_get(map, "draconis", &found), 1);
	ASSERT_EQ_INT(found, 1);

	ASSERT_EQ_INT(map_get(map, "github", &found), 2);
	ASSERT_EQ_INT(found, 1);

	ASSERT_EQ_INT(map_get(map, "missing", &found), -1);
	ASSERT_EQ_INT(found, 0);

	free_hashmap(map);
}

// Inserting the same key twice updates the value instead of adding a second
// entry.
TEST(test_hashmap_insert_updates_existing_key) {
	struct HashMap *map = create_hashmap();
	int found = 0;

	map_insert(map, "draconis", 1);
	map_insert(map, "draconis", 3);

	ASSERT_EQ_INT(map_get(map, "draconis", &found), 3);
	ASSERT_EQ_INT(found, 1);

	free_hashmap(map);
}

// A stored value of -1 is still distinguishable from "not found".
TEST(test_hashmap_reports_negative_values) {
	struct HashMap *map = create_hashmap();
	int found = 0;

	map_insert(map, "negative", -1);

	ASSERT_EQ_INT(map_get(map, "negative", &found), -1);
	ASSERT_EQ_INT(found, 1);

	free_hashmap(map);
}

// Removing a key makes it unreachable again.
TEST(test_hashmap_remove) {
	struct HashMap *map = create_hashmap();
	int found = 0;

	map_insert(map, "draconis", 1);
	map_insert(map, "github", 2);

	map_remove(map, "draconis");
	ASSERT_EQ_INT(map_get(map, "draconis", &found), -1);
	ASSERT_EQ_INT(found, 0);

	// The other entry is untouched.
	ASSERT_EQ_INT(map_get(map, "github", &found), 2);
	ASSERT_EQ_INT(found, 1);

	// Removing a key that is not there is harmless.
	map_remove(map, "missing");
	ASSERT_EQ_INT(map_get(map, "github", &found), 2);

	free_hashmap(map);
}

// The map keeps its own copy of the key, so the caller's buffer can change.
TEST(test_hashmap_owns_the_key) {
	struct HashMap *map = create_hashmap();
	char key[] = "mutable";
	int found = 0;

	map_insert(map, key, 5);

	// Change the caller's buffer after the insert.
	key[0] = 'M';

	ASSERT_EQ_INT(map_get(map, "mutable", &found), 5);
	ASSERT_EQ_INT(found, 1);

	ASSERT_EQ_INT(map_get(map, "Mutable", &found), -1);
	ASSERT_EQ_INT(found, 0);

	free_hashmap(map);
}

// The hash always lands inside the bucket array.
TEST(test_hashmap_hash_stays_in_range) {
	ASSERT(hashmap_hash("draconis") < BUCKETS);
	ASSERT(hashmap_hash("") < BUCKETS);
	ASSERT(hashmap_hash("a") < BUCKETS);

	// The same key always hashes to the same bucket.
	ASSERT_EQ_UINT(hashmap_hash("draconis"), hashmap_hash("draconis"));
}

// Freeing is safe with a NULL pointer.
TEST(test_hashmap_free_is_null_safe) {
	free_hashmap(NULL);
}

// Runs every hash map test case.
void run_hashmap_tests(void) {
	test_section("hashmap");

	RUN_TEST(test_hashmap_create_is_empty);
	RUN_TEST(test_hashmap_insert_and_get);
	RUN_TEST(test_hashmap_insert_updates_existing_key);
	RUN_TEST(test_hashmap_reports_negative_values);
	RUN_TEST(test_hashmap_remove);
	RUN_TEST(test_hashmap_owns_the_key);
	RUN_TEST(test_hashmap_hash_stays_in_range);
	RUN_TEST(test_hashmap_free_is_null_safe);
}
