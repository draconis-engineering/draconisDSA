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

#ifndef DLLIST_H
#define DLLIST_H

/**
 * @brief A node in a doubly linked list.
 */
struct DLLNode {
	/** Pointer to the data stored in this node. Owned by caller. */
	void *data;
	/** Pointer to the next node in the list. */
	struct DLLNode *next;
	/** Pointer to the previous node in the list. */
	struct DLLNode *prev;
};

/**
 * @brief A doubly linked list container.
 */
struct DLList {
	/** Number of nodes in the list. */
	int size;
	/** Pointer to the first node in the list. */
	struct DLLNode *head;
	/** Pointer to the last node in the list. */
	struct DLLNode *tail;
};

/**
 * @brief Create and initialize an empty doubly linked list.
 * @return A pointer to the new list, or NULL on allocation failure.
 */
struct DLList *create_dll();

/**
 * @brief Check if the list is empty.
 * @param l Pointer to the list.
 * @return 1 if empty, 0 otherwise.
 */
int dll_is_empty(struct DLList *l);

/**
 * @brief Get the index of the first occurrence of a value.
 * @param l Pointer to the list.
 * @param val Pointer to the value to search for (compared by pointer equality).
 * @return Index of the value, or -1 if not found.
 */
int dll_get_idx(struct DLList *l, void *val);

/**
 * @brief Get the number of elements in the list.
 * @param l Pointer to the list.
 * @return Size of the list.
 */
int dll_size(struct DLList *l);

/**
 * @brief Remove the node at the specified index.
 * @param l Pointer to the list.
 * @param idx Index of the node to remove (0 <= idx < size).
 */
void dll_remove(struct DLList *l, int idx);

/**
 * @brief Insert a value at the specified index.
 * @param l Pointer to the list.
 * @param val Pointer to the value to insert.
 * @param idx Index to insert at (0 <= idx <= size, or -1 to insert at back).
 */
void dll_insert(struct DLList *l, void *val, int idx);

/**
 * @brief Insert a value at the front of the list.
 * @param l Pointer to the list.
 * @param val Pointer to the value to insert.
 */
void dll_insert_at_front(struct DLList *l, void *val);

/**
 * @brief Insert a value at the back of the list.
 * @param l Pointer to the list.
 * @param val Pointer to the value to insert.
 */
void dll_insert_at_back(struct DLList *l, void *val);

/**
 * @brief Print the list contents as pointer addresses.
 * @param l Pointer to the list.
 */
void print_dll(struct DLList *l);

/**
 * @brief Free all nodes in the list and the list structure itself.
 * @param l Pointer to the list.
 */
void free_dll(struct DLList *l);

#endif
