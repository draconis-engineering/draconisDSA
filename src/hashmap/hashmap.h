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

#ifndef HASHMAP_H
#define HASHMAP_H

#define BUCKETS 16

/**
 * @brief A node in the hash map's separate chaining list.
 */
struct HashMapNode {
	/** Key string (owned by the map). */
	char *key;
	/** Integer value associated with the key. */
	int value;
	/** Pointer to the next node in the chain. */
	struct HashMapNode *next;
};

/**
 * @brief A string-keyed hash map.
 */
struct HashMap {
	/** Array of bucket heads for separate chaining. */
	struct HashMapNode *buckets[BUCKETS];
};

/**
 * @brief Compute the hash index for a given key using DJB2.
 * @param key The input string key.
 * @return Index into the buckets array.
 */
unsigned int hash(const char *key);

/**
 * @brief Create and initialize an empty hash map.
 * @return A pointer to the new map, or NULL on allocation failure.
 */
struct HashMap *create_hashmap();

/**
 * @brief Insert or update a key-value pair.
 * @param map Pointer to the hash map.
 * @param key Key string.
 * @param value Value to associate with the key.
 */
void map_insert(struct HashMap *map, const char *key, int value);

/**
 * @brief Retrieve the value associated with a key.
 * @param map Pointer to the hash map.
 * @param key Key string to look up.
 * @param found Set to 1 if found, 0 if not found.
 * @return The value if found, otherwise -1.
 */
int map_get(struct HashMap *map, const char *key, int *found);

/**
 * @brief Remove a key-value pair from the map if it exists.
 * @param map Pointer to the hash map.
 * @param key Key string to remove.
 */
void map_remove(struct HashMap *map, const char *key);

/**
 * @brief Free all memory used by the hash map.
 * @param map Pointer to the hash map.
 */
void free_hashmap(struct HashMap *map);

#endif // HASHMAP_H
