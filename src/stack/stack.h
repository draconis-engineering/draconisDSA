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

#include "../dllist/dllist.h"

/**
 * @brief A LIFO (Last In, First Out) stack.
 *
 * The element count is tracked by the underlying list (`entries->size`).
 */
struct Stack {
	/** Underlying doubly linked list for storing entries. */
	struct DLList *entries;
};

/**
 * @brief Create and initialize an empty stack.
 * @return A pointer to the new stack, or NULL on allocation failure.
 */
struct Stack *create_stack();

/**
 * @brief Push a value onto the top of the stack.
 * @param stack Pointer to the stack.
 * @param data Pointer to the data to push.
 */
void stack_push(struct Stack *stack, void *data);

/**
 * @brief Pop the top value from the stack.
 * @param stack Pointer to the stack.
 * @return Pointer to the popped data, or NULL if the stack is empty.
 */
void *stack_pop(struct Stack *stack);

/**
 * @brief Peek at the top value without removing it.
 * @param stack Pointer to the stack.
 * @return Pointer to the top data, or NULL if the stack is empty.
 */
void *stack_peek(struct Stack *stack);

/**
 * @brief Free the stack and all its entries.
 * @param stack Pointer to the stack.
 */
void free_stack(struct Stack *stack);

#endif // stack.h
