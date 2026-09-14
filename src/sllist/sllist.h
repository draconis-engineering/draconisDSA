/*
* Singly Linked List Library - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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


#ifndef DLLIST_H
#define DLLIST_H

struct Node {
    int data;
    struct Node* next;
};

struct SLList {
    int size;
    struct Node* head;
    struct Node* tail;
};

struct SLList* init_list();
int is_empty(struct SLList *l);
int get_idx(struct SLList *l, int val);
int list_size(struct SLList *l);
void list_remove(struct SLList *l, int idx);
void insert(struct SLList *l, int val, int idx);
void insert_at_front(struct SLList *l, int val);
void insert_at_back(struct SLList *l, int val);
void print_list(struct SLList *l);
void free_list(struct SLList *l);

#endif
