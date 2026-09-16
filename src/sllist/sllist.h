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

#include <stdbool.h>

struct SLLNode {
	void *data;
	struct SLLNode *next;
};

struct SLList {
	int size;
	struct SLLNode *head;
	struct SLLNode *tail;
};

struct SLList *create_sllist();
bool sll_is_empty(struct SLList *l);
int sll_get_idx(struct SLList *l, void *val);
int sll_list_size(struct SLList *l);
void sll_remove(struct SLList *l, int idx);
void sll_insert(struct SLList *l, void *val, int idx);
void sll_insert_at_front(struct SLList *l, void *val);
void sll_insert_at_back(struct SLList *l, void *val);
void print_sllist(struct SLList *l);
void free_sllist(struct SLList *l);

#endif
