/*
 * Doubly Linked List Library - DRACONIS ENGINEERING DATA STRUCTURES &
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

#include "dllist.h"
#include <stdio.h>
#include <stdlib.h>

// Initializes the list.
struct DLList *create_dllist() {
	struct DLList *l = malloc(sizeof(struct DLList));
	if (l == NULL) {
		return NULL;
	}
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
	return l;
}

// Returns whether the list is empty.
int dll_is_empty(struct DLList *l) { return (l->head == NULL); }

// Returns the index of the first occurrence of the given value in the list.
int dll_get_idx(struct DLList *l, void *val) {
	struct DLLNode *curr_node = l->head;
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
int dll_list_size(struct DLList *l) { return l->size; }

// Inserts a value at the front of the list.
void dll_insert_at_front(struct DLList *l, void *val) {
	struct DLLNode *new_node = (struct DLLNode *)malloc(sizeof(struct DLLNode));
	if (new_node == NULL) {
		printf("Error: malloc failed\n");
		return;
	}

	new_node->data = val;
	new_node->prev = NULL;
	new_node->next = l->head;

	if (l->head != NULL) {
		l->head->prev = new_node;

	} else {
		l->tail = new_node;
	}

	l->head = new_node;
	l->size++;
}

// Inserts a value at the back of the list.
void dll_insert_at_back(struct DLList *l, void *val) {
	struct DLLNode *new_node = (struct DLLNode *)malloc(sizeof(struct DLLNode));
	if (new_node == NULL) {
		printf("Error: malloc failed\n");
		return;
	}

	new_node->data = val;
	new_node->prev = l->tail;
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
void print_list(struct DLList *l) {
	printf("[");
	struct DLLNode *curr_node = l->head;
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
void dll_insert(struct DLList *l, void *val, int idx) {
	int size = dll_list_size(l);
	if (idx < 0 || idx > size) {
		printf("List Index out of range!\n");
		return;
	}

	if (idx == 0) {
		dll_insert_at_front(l, val);
		return;
	}

	if (idx == size || idx == -1) {
		dll_insert_at_back(l, val);
		return;
	}

	struct DLLNode *new_node = (struct DLLNode *)malloc(sizeof(struct DLLNode));
	if (new_node == NULL) {
		printf("Error: malloc failed\n");
		return;
	}

	new_node->data = val;

	// Walk to the node currently sitting at idx and link the new node in
	// front of it.
	struct DLLNode *curr_node = l->head;
	for (int i = 0; i < idx; i++) {
		curr_node = curr_node->next;
	}

	new_node->prev = curr_node->prev;
	new_node->next = curr_node;
	curr_node->prev->next = new_node;
	curr_node->prev = new_node;
	l->size++;
}

// Removes the node at the given index from the list.
void dll_remove(struct DLList *l, int idx) {
	if ((idx + 1) > dll_list_size(l)) {
		printf("List Index out of range!");
		return;
	}

	struct DLLNode *curr_node = l->head;
	int curr_idx = 0;

	if (idx == 0) {
		struct DLLNode *temp = l->head;
		l->head = l->head->next;
		if (l->head != NULL) {
			l->head->prev = NULL;
		} else {
			l->tail = NULL;
		}
		free(temp);
		l->size--;
		return;

	} else if (idx == dll_list_size(l) - 1) {
		struct DLLNode *temp = l->tail;
		l->tail = l->tail->prev;
		if (l->tail != NULL) {
			l->tail->next = NULL;
		} else {
			l->head = NULL;
		}
		free(temp);
		l->size--;
		return;
	}

	while (curr_node != NULL) {
		if (curr_idx == idx) {
			curr_node->next->prev = curr_node->prev;
			curr_node->prev->next = curr_node->next;
			free(curr_node);
			l->size--;
			return;
		}

		curr_node = curr_node->next;
		curr_idx++;
	}
}

// Frees the memory used by the list. Does not free the list itself.
void free_list(struct DLList *l) {
	struct DLLNode *curr_node = l->head;

	while (curr_node != NULL) {
		struct DLLNode *to_rem = curr_node->next;
		free(curr_node);
		curr_node = to_rem;
	}

	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
	free(l);
}
