/* DOUBLY LINKED LIST LIBRARY - DURENDAL ENGINEERING - V.1 */

#include <stdio.h>
#include <stdlib.h>
#include "dllist.h"

// Initializes the list.
void init_list(List *l) {
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

// Returns whether the list is empty.
int is_empty(List *l) {
    return (l->head == NULL);
}

// Returns the index of the first occurrence of the given value in the list.
int get_idx(List *l, int val) {
    Node* curr_node = l->head;
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
int list_size(List *l) {
    return l->size;
}

// Inserts a value at the front of the list.
void insert_at_front(List *l, int val) {
    Node* new_node = (Node*)malloc(sizeof(Node));
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
void insert_at_back(List *l, int val) {
    Node* new_node = (Node*)malloc(sizeof(Node));
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
void print_list(List *l) {
    printf("[");
    Node* curr_node = l->head;
    while (curr_node != NULL) {
        printf("%d", curr_node->data);
        if (curr_node->next != NULL) {
            printf(", ");
        }
        curr_node = curr_node->next;
    }

    printf("]\n");
}

// Inserts a value at the given index in the list.
void insert(List *l, int val, int idx) {
    if ((idx + 1) > list_size(l)) {
        printf("List Index out of range!");
        return;
    }

    Node* curr_node = l->head;
    Node* new_node = (Node*)malloc(sizeof(Node));

    if (new_node == NULL) { printf("Error: malloc failed\n"); return; }

    new_node->data = val;

    int curr_idx = 0;

    if (idx == 0) {
        insert_at_front(l, val);
        return;
    } else if (idx == list_size(l) - 1) {
        insert_at_back(l, val);
        return;
    }

    while (curr_node->next != NULL) {
        if (curr_idx == idx) {
            new_node->prev = curr_node->prev;
            new_node->next = curr_node;
            curr_node->prev->next = new_node;
            curr_node->prev = new_node;
            l->size++;
            return;
        }

        curr_node = curr_node->next;
        curr_idx++;
    }
}

// Removes the node at the given index from the list.
void list_remove(List *l, int idx) {
    if ((idx + 1) > list_size(l)) {
        printf("List Index out of range!");
        return;
    }

    Node* curr_node = l->head;
    int curr_idx = 0;

    if (idx == 0) {
        Node* temp = l->head;
        l->head = l->head->next;
        if (l->head != NULL) {l->head->prev = NULL;} else {l->tail = NULL;}
        free(temp);
        l->size--;
        return;

    } else if (idx == list_size(l) - 1) {
        Node* temp = l->tail;
        l->tail = l->tail->prev;
        if (l->tail != NULL) {l->tail->next = NULL;} else {l->head = NULL;}
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

// Frees the memory used by the list.
void free_list(List *l) {
    Node* curr_node = l->head;

    while (curr_node != NULL) {
        Node* to_rem = curr_node->next;
        free(curr_node);
        curr_node = to_rem;
    }

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}
