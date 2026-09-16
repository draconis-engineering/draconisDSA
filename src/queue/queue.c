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

#include "queue.h"
#include <stdlib.h>

struct Queue *create_queue() {
	struct Queue *queue = malloc(sizeof(struct Queue));
	if (queue == NULL) {
		return NULL;
	}
	queue->entries = create_dllist();
	queue->size = 0;
	return queue;
}

void queue_add(struct Queue *queue, void *data) {
	dll_insert_at_back(queue->entries, data);
	queue->size++;
}

void *queue_get(struct Queue *queue) {
	if (queue->size == 0) {
		return NULL;
	}
	void *data = queue->entries->head->data;
	dll_remove(queue->entries, 0);
	queue->size--;
	return data;
}

void *queue_peek(struct Queue *queue) {
	if (queue->size == 0) {
		return NULL;
	}
	return queue->entries->head->data;
}

void free_queue(struct Queue *queue) {
	free_list(queue->entries);
	free(queue);
}
