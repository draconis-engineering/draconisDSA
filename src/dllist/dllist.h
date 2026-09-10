/*
 * Doubly Linked List
 *
 * Draconis Engineering
 */

#ifndef DLLIST_H
#define DLLIST_H

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct DLList {
    int size;
    Node* head;
    Node* tail;
} List;

void init_list(List *l);
int is_empty(List *l);
int get_idx(List *l, int value);
int list_size(List *l);
void list_remove(List *l, int Idx);
void insert(List *l, int value, int Idx);
void insert_at_front(List *l, int value);
void insert_at_back(List *l, int value);
void print_list(List *l);
void free_list(List *l);

#endif
