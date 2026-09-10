/* DOUBLY LINKED LIST LIBRARY - DURENDAL ENGINEERING - V.1 */

#include <stdio.h>
#include <stdlib.h>
#include "dllist.h"

void init_list(List *l) {
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

int is_empty(List *l) {
    return (l->head == NULL);
}

int get_idx(List *l, int value) {
    Node* CurrentNode = l->head;
    int index = 0;

    while (CurrentNode != NULL) {
        if (CurrentNode->data == value) {
            return index;
        }

        CurrentNode = CurrentNode->next;
        index++;
    }

    return -1;
}

int list_size(List *l) {
    return l->size;
}

void insert_at_front(List *l, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Error: malloc failed\n");
        return;
    }

    newNode->data = value;
    newNode->prev = NULL;
    newNode->next = l->head;

    if (l->head != NULL) {
        l->head->prev = newNode;

    } else {
        l->tail = newNode;
    }

    l->head = newNode;
    l->size++;
}
void insert_at_back(List *l, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Error: malloc failed\n");
        return;
    }

    newNode->data = value;
    newNode->prev = l->tail;
    newNode->next = NULL;

    if (l->tail != NULL) {
        l->tail->next = newNode;
    } else {
        l->head = newNode;
    }

    l->tail = newNode;
    l->size++;

}
void print_list(List *l) {
    printf("[");
    Node* CurrentNode = l->head;
    while (CurrentNode != NULL) {
        printf("%d", CurrentNode->data);
        if (CurrentNode->next != NULL) {
            printf(", ");
        }
        CurrentNode = CurrentNode->next;
    }

    printf("]\n");
}
void insert(List *l, int value, int Idx) {
    if ((Idx + 1) > list_size(l)) {
        printf("List Index out of range!");
        return;
    }

    Node* CurrentNode = l->head;
    Node* newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) { printf("Error: malloc failed\n"); return; }

    newNode->data = value;

    int CurrentIdx = 0;

    if (Idx == 0) {
        insert_at_front(l, value);
        return;
    } else if (Idx == list_size(l) - 1) {
        insert_at_back(l, value);
        return;
    }

    while (CurrentNode->next != NULL) {
        if (CurrentIdx == Idx) {
            newNode->prev = CurrentNode->prev;
            newNode->next = CurrentNode;
            CurrentNode->prev->next = newNode;
            CurrentNode->prev = newNode;
            l->size++;
            return;
        }

        CurrentNode = CurrentNode->next;
        CurrentIdx++;
    }
}
void list_remove(List *l, int Idx) {
    if ((Idx + 1) > list_size(l)) {
        printf("List Index out of range!");
        return;
    }

    Node* CurrentNode = l->head;
    int CurrentIdx = 0;

    if (Idx == 0) {
        Node* temp = l->head;
        l->head = l->head->next;
        if (l->head != NULL) {l->head->prev = NULL;} else {l->tail = NULL;}
        free(temp);
        l->size--;
        return;

    } else if (Idx == list_size(l) - 1) {
        Node* temp = l->tail;
        l->tail = l->tail->prev;
        if (l->tail != NULL) {l->tail->next = NULL;} else {l->head = NULL;}
        free(temp);
        l->size--;
        return;
    }

    while (CurrentNode != NULL) {
        if (CurrentIdx == Idx) {
            CurrentNode->next->prev = CurrentNode->prev;
            CurrentNode->prev->next = CurrentNode->next;
            free(CurrentNode);
            l->size--;
            return;
        }

        CurrentNode = CurrentNode->next;
        CurrentIdx++;
    }
}
void free_list(List *l) {
    Node* CurrentNode = l->head;

    while (CurrentNode != NULL) {
        Node* NextToRemove = CurrentNode->next;
        free(CurrentNode);
        CurrentNode = NextToRemove;
    }

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}
