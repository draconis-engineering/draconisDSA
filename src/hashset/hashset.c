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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashset.h"

// Portable replacement for POSIX strdup / MSVC _strdup.
static char *dup_string(const char *src) {
	size_t len = strlen(src) + 1;
	char *copy = malloc(len);
	if (copy != NULL) {
		memcpy(copy, src, len);
	}
	return copy;
}

// Create a new HashSet
struct HashSet *create_hashset() {
	struct HashSet *set = (struct HashSet *)malloc(sizeof(struct HashSet));
	if (!set) {
		fprintf(stderr, "Error: malloc failed\n");
		return NULL;
	}

	set->capacity = INITIAL_CAPACITY;
	set->size = 0;
	set->buckets =
		calloc(set->capacity,
			   sizeof(struct HashSetNode *)); // Initialize all buckets to NULL
	if (!set->buckets) {
		free(set);
		return NULL;
	}
	return set;
}

// DJB2 Hash Function - distributes strings cleanly across buckets
unsigned int hashset_hash(struct HashSet *set, const char *value) {
	unsigned long hash = 0x1505;
	int c;
	while ((c = *value++)) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash % set->capacity;
}

// Check if value exists in the set
bool hashset_contains(struct HashSet *set, const char *value) {
	if (!set || !value) {
		return false;
	}

	unsigned int index = hashset_hash(set, value);
	struct HashSetNode *current = set->buckets[index];

	while (current != NULL) {
		if (strcmp(current->value, value) == 0) {
			return true; // Found
		}
		current = current->next;
	}
	return false;
}

// Add a value to the set (Returns false if duplicate or error)
bool hashset_add(struct HashSet *set, const char *value) {
	if (!set || !value) {
		return false;
	}

	if (hashset_contains(set, value)) {
		return false; // HashSets do not allow duplicates
	}

	unsigned int index = hashset_hash(set, value);

	// Allocate new node
	struct HashSetNode *new_node = malloc(sizeof(struct HashSetNode));
	if (!new_node) {
		return false;
	}

	new_node->value = dup_string(value); // Duplicate the string into memory
	if (!new_node->value) {
		free(new_node);
		return false;
	}

	// Insert at the head of the linked list (Separate Chaining)
	new_node->next = set->buckets[index];
	set->buckets[index] = new_node;
	set->size++;

	return true;
}

// Remove a value from the set
bool hashset_remove(struct HashSet *set, const char *value) {
	if (!set || !value) {
		return false;
	}

	unsigned int index = hashset_hash(set, value);
	struct HashSetNode *current = set->buckets[index];
	struct HashSetNode *prev = NULL;

	while (current != NULL) {
		if (strcmp(current->value, value) == 0) {
			if (prev == NULL) {
				set->buckets[index] = current->next;
			} else {
				prev->next = current->next;
			}
			free(current->value);
			free(current);
			set->size--;
			return true;
		}
		prev = current;
		current = current->next;
	}
	return false; // Not found
}

// Free all memory used by the HashSet
void hashset_destroy(struct HashSet *set) {
	if (!set) {
		return;
	}

	for (size_t i = 0; i < set->capacity; i++) {
		struct HashSetNode *current = set->buckets[i];
		while (current != NULL) {
			struct HashSetNode *temp = current;
			current = current->next;
			free(temp->value);
			free(temp);
		}
	}
	free(set->buckets);
	free(set);
}
