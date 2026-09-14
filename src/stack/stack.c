/*
* Stack Library - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "stack.h"
#include "../dllist/dllist.h"

#include <stdlib.h>

struct Stack* stack_init() {
    struct Stack* stack = malloc(sizeof(struct Stack));
    struct DLList *entries = init_list();

    if (stack == NULL || entries == NULL) {
        return NULL;
    }

    stack->entries = entries;
    stack->size = 0;
    stack->capacity = 0;
    return stack;
}

void stack_push(struct Stack* stack, int data) {
    insert_at_back(&(stack->entries[stack->size]), data);
    stack->size++;
}

int stack_pop(struct Stack* stack) {}

int stack_peek(struct Stack* stack) {
    if (stack->size == 0) {
        return NULL;
    }
    return stack->entries->tail->data;
}
