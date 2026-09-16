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

#include "hashmap.h"
#include <stdlib.h>
#include <string.h>


// Initialize the map
struct HashMap *create_map() {
	struct HashMap *map = malloc(sizeof(struct HashMap));
	for (int i = 0; i < BUCKETS; i++) {
		map->buckets[i] = NULL;
	}
	return map;
}

// DJB2 Hashing Algorithm (Simple and highly effective for strings)
unsigned int djb2(const char *key) {
	unsigned long int v = 5381;
	int i = 0;
	while (key[i] != '\0') {
		v = ((v << 5) + v) + key[i]; // value * 33 + key[i]
		i++;
	}
	return v % BUCKETS;
}

// Insert or Update a key-value pair
void map_insert(struct HashMap *map, const char *key, int val) {
	unsigned int idx = djb2(key);
	struct Node *current = map->buckets[idx];

	// Check if key already exists, if so update it
	while (current != NULL) {
		if (strcmp(current->key, key) == 0) {
			current->value = val;
			return;
		}
		current = current->next;
	}

	// Key doesn't exist, create a new node and prepend it (O(1) insertion)
	struct Node *new_node = malloc(sizeof(struct Node));
	new_node->key = strdup(key); // Duplicate string to manage memory safely
	new_node->value = val;
	new_node->next = map->buckets[idx];
	map->buckets[idx] = new_node;
}

// Retrieve a value
int map_get(struct HashMap *map, const char *key, int *found) {
	unsigned int idx = djb2(key);
	struct Node *current = map->buckets[idx];

	while (current != NULL) {
		if (strcmp(current->key, key) == 0) {
			*found = 1;
			return current->value;
		}
		current = current->next;
	}
	*found = 0;
	return -1; // Default error return
}
