/*
 * HashMap Library - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>
#include <stddef.h>

#define INITIAL_CAPACITY 16

/**
 * @brief A node in the hash set's separate chaining list.
 */
struct HashSetNode {
	/** Value string (owned by the set). */
	char *value;
	/** Pointer to the next node in the chain. */
	struct HashSetNode *next;
};

/**
 * @brief A hash set for storing unique string values.
 */
struct HashSet {
	/** Array of bucket heads for separate chaining. */
	struct HashSetNode **buckets;
	/** Number of buckets in the set. */
	size_t capacity;
	/** Number of elements in the set. */
	size_t size;
};

/**
 * @brief Create and initialize an empty hash set.
 * @return A pointer to the new set, or NULL on allocation failure.
 */
struct HashSet *create_hashset();

/**
 * @brief Compute the hash index for a given value using DJB2.
 * @param set Pointer to the hash set.
 * @param value The input string value.
 * @return Index into the buckets array.
 */
unsigned int hashset_hash(struct HashSet *set, const char *value);

/**
 * @brief Check if a value exists in the set.
 * @param set Pointer to the hash set.
 * @param value The value to look up.
 * @return True if the value exists, false otherwise.
 */
bool hashset_contains(struct HashSet *set, const char *value);

/**
 * @brief Add a value to the set.
 * @param set Pointer to the hash set.
 * @param value The value to add.
 * @return True if added, false if already exists or on allocation failure.
 */
bool hashset_add(struct HashSet *set, const char *value);

/**
 * @brief Remove a value from the set.
 * @param set Pointer to the hash set.
 * @param value The value to remove.
 * @return True if removed, false if not found.
 */
bool hashset_remove(struct HashSet *set, const char *value);

/**
 * @brief Free all memory used by the hash set.
 * @param set Pointer to the hash set.
 */
void hashset_destroy(struct HashSet *set);

#endif // hashset.h
