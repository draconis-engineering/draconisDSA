/*
 * Singly Linked List Library - DRACONIS ENGINEERING DATA STRUCTURES &
 * ALGORITHMS Copyright (C) 2026 Simon Stordal Amundgård
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

#include "sllist.h"
#include <stdio.h>
#include <stdlib.h>

// Initializes the list.
struct SLList *create_sllist() {
	struct SLList *l = (struct SLList *)malloc(sizeof(struct SLList));
	if (l == NULL) {
		return NULL;
	}
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
	return l;
}

// Returns whether the list is empty.
bool sll_is_empty(struct SLList *l) { return (l->size == 0); }

// Returns the index of the first occurrence of the given value in the list.
int sll_get_idx(struct SLList *l, void *val) {
	struct SLLNode *curr_node = l->head;
	int index = 0;

	while (curr_node != NULL) {
		if (curr_node->data == val) {
			return index;
		}

		curr_node = curr_node->next;
		index++;
	}

	return -1;
}

// Returns the size of the list.
int sll_list_size(struct SLList *l) { return l->size; }

// Inserts a value at the front of the list.
void sll_insert_at_front(struct SLList *l, void *val) {
	struct SLLNode *new_node = (struct SLLNode *)malloc(sizeof(struct SLLNode));
	if (new_node == NULL) {
		printf("Error: malloc failed\n");
		return;
	}

	new_node->data = val;
	new_node->next = l->head;

	l->head = new_node;

	if (l->tail == NULL) {
		l->tail = new_node;
	}

	l->size++;
}

// Inserts a value at the back of the list.
void sll_insert_at_back(struct SLList *l, void *val) {
	struct SLLNode *new_node = (struct SLLNode *)malloc(sizeof(struct SLLNode));
	if (new_node == NULL) {
		printf("Error: malloc failed\n");
		return;
	}

	new_node->data = val;
	new_node->next = NULL;

	if (l->tail != NULL) {
		l->tail->next = new_node;
	} else {
		l->head = new_node;
	}

	l->tail = new_node;
	l->size++;
}

// Prints the list to the console.
void print_sllist(struct SLList *l) {
	printf("[");
	struct SLLNode *curr_node = l->head;
	while (curr_node != NULL) {
		printf("%p", curr_node->data);
		if (curr_node->next != NULL) {
			printf(", ");
		}
		curr_node = curr_node->next;
	}

	printf("]\n");
}

// Inserts a value at the given index in the list. Valid indices are 0..size.
void sll_insert(struct SLList *l, void *val, int idx) {
	int size = sll_list_size(l);
	if (idx < 0 || idx > size) {
		printf("List Index out of range!\n");
		return;
	}

	if (idx == 0) {
		sll_insert_at_front(l, val);
		return;
	}

	if (idx == size || idx == -1) {
		sll_insert_at_back(l, val);
		return;
	}

	struct SLLNode *new_node = (struct SLLNode *)malloc(sizeof(struct SLLNode));
	if (new_node == NULL) {
		printf("Error: malloc failed\n");
		return;
	}

	new_node->data = val;

	// Walk to the node just before the insertion point. It is guaranteed to
	// exist because idx is in [1, size-1].
	struct SLLNode *curr_node = l->head;
	for (int i = 0; i < idx - 1; i++) {
		curr_node = curr_node->next;
	}

	new_node->next = curr_node->next;
	curr_node->next = new_node;
	l->size++;
}

// Removes the node at the given index from the list.
void sll_remove(struct SLList *l, int idx) {
	int size = sll_list_size(l);
	if (idx < 0 || idx >= size) {
		printf("List Index out of range!\n");
		return;
	}

	struct SLLNode *to_remove;

	if (idx == 0) {
		to_remove = l->head;
		l->head = l->head->next;
		if (l->head == NULL) {
			l->tail = NULL;
		}
		free(to_remove);
		l->size--;
		return;
	}

	// Walk to the node just before the one to remove.
	struct SLLNode *curr_node = l->head;
	for (int i = 0; i < idx - 1; i++) {
		curr_node = curr_node->next;
	}

	to_remove = curr_node->next;
	curr_node->next = to_remove->next;

	if (to_remove == l->tail) {
		l->tail = curr_node;
	}

	free(to_remove);
	l->size--;
}

// Frees the memory used by the list's nodes. Does not free the list itself.
void free_sllist(struct SLList *l) {
	struct SLLNode *curr_node = l->head;

	while (curr_node != NULL) {
		struct SLLNode *next = curr_node->next;
		free(curr_node);
		curr_node = next;
	}

	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
	free(l);
}
