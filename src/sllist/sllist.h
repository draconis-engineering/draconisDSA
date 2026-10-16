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

#ifndef SLLIST_H
#define SLLIST_H

/**
 * @brief A node in a singly linked list.
 */
struct SLLNode {
	/** Pointer to the data stored in this node. Owned by caller. */
	void *data;
	/** Pointer to the next node in the list. */
	struct SLLNode *next;
};

/**
 * @brief A singly linked list container.
 */
struct SLList {
	/** Number of nodes in the list. */
	int size;
	/** Pointer to the first node in the list. */
	struct SLLNode *head;
	/** Pointer to the last node in the list. */
	struct SLLNode *tail;
};

/**
 * @brief Create and initialize an empty singly linked list.
 * @return A pointer to the new list, or NULL on allocation failure.
 */
struct SLList *create_sll();

/**
 * @brief Check if the list is empty.
 * @param l Pointer to the list.
 * @return 1 if empty, 0 otherwise.
 */
int sll_is_empty(struct SLList *l);

/**
 * @brief Get the index of the first occurrence of a value.
 * @param l Pointer to the list.
 * @param val Pointer to the value to search for (compared by pointer equality).
 * @return Index of the value, or -1 if not found.
 */
int sll_get_idx(struct SLList *l, void *val);

/**
 * @brief Get the number of elements in the list.
 * @param l Pointer to the list.
 * @return Size of the list.
 */
int sll_size(struct SLList *l);

/**
 * @brief Remove the node at the specified index.
 * @param l Pointer to the list.
 * @param idx Index of the node to remove (0 <= idx < size).
 */
void sll_remove(struct SLList *l, int idx);

/**
 * @brief Insert a value at the specified index.
 * @param l Pointer to the list.
 * @param val Pointer to the value to insert.
 * @param idx Index to insert at (0 <= idx <= size, or -1 to insert at back).
 */
void sll_insert(struct SLList *l, void *val, int idx);

/**
 * @brief Insert a value at the front of the list.
 * @param l Pointer to the list.
 * @param val Pointer to the value to insert.
 */
void sll_insert_at_front(struct SLList *l, void *val);

/**
 * @brief Insert a value at the back of the list.
 * @param l Pointer to the list.
 * @param val Pointer to the value to insert.
 */
void sll_insert_at_back(struct SLList *l, void *val);

/**
 * @brief Print the list contents as pointer addresses.
 * @param l Pointer to the list.
 */
void print_sll(struct SLList *l);

/**
 * @brief Free all nodes in the list and the list structure itself.
 * @param l Pointer to the list.
 */
void free_sll(struct SLList *l);

#endif // sllist.h
