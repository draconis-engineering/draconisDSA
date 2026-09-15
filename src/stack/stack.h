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

#ifndef STACK_H
#define STACK_H

#define INITIAL_CAPACITY 16

struct Stack {
  struct DLList *entries;
  size_t size;
  size_t capacity;
};

struct Stack *stack_init();
void stack_push(struct Stack *stack, void *data);
void *stack_pop(struct Stack *stack);
void *stack_peek(struct Stack *stack);

#endif // stack.h
