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

struct DLLNode {
	void *data;
	struct DLLNode *next;
	struct DLLNode *prev;
};

struct DLList {
	int size;
	struct DLLNode *head;
	struct DLLNode *tail;
};

struct DLList *create_dllist();
int dll_is_empty(struct DLList *l);
int dll_get_idx(struct DLList *l, void *val);
int dll_list_size(struct DLList *l);
void dll_remove(struct DLList *l, int idx);
void dll_insert(struct DLList *l, void *val, int idx);
void dll_insert_at_front(struct DLList *l, void *val);
void dll_insert_at_back(struct DLList *l, void *val);
void print_list(struct DLList *l);
void free_list(struct DLList *l);

#endif
