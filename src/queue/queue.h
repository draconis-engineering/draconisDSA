/*
 * Queue Library - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#ifndef QUEUE_H
#define QUEUE_H

#include "../dllist/dllist.h"

/**
 * @brief A FIFO (First In, First Out) queue.
 *
 * The element count is tracked by the underlying list (`entries->size`).
 */
struct Queue {
	/** Underlying doubly linked list for storing entries. */
	struct DLList *entries;
};

/**
 * @brief Create and initialize an empty queue.
 * @return A pointer to the new queue, or NULL on allocation failure.
 */
struct Queue *create_queue();

/**
 * @brief Add an element to the rear of the queue (enqueue).
 * @param queue Pointer to the queue.
 * @param data Pointer to the data to add.
 */
void queue_add(struct Queue *queue, void *data);

/**
 * @brief Remove an element from the front of the queue (dequeue).
 * @param queue Pointer to the queue.
 * @return Pointer to the dequeued data, or NULL if the queue is empty.
 */
void *queue_get(struct Queue *queue);

/**
 * @brief Peek at the front of the queue without removing it.
 * @param queue Pointer to the queue.
 * @return Pointer to the front data, or NULL if the queue is empty.
 */
void *queue_peek(struct Queue *queue);

/**
 * @brief Free the queue and all its entries.
 * @param queue Pointer to the queue.
 */
void free_queue(struct Queue *queue);

#endif // queue.h
