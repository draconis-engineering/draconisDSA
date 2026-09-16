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

struct Queue *init_queue() {
	struct Queue *queue = malloc(sizeof(struct Queue));
	if (queue == NULL) {
		return NULL;
	}
	queue->entries = init_list();
	queue->size = 0;
	return queue;
}

void queue_add(struct Queue *queue, void *data) {
	insert_at_back(queue->entries, data);
	queue->size++;
}

void *queue_get(struct Queue *queue) {
	void *data = queue->entries->head->data;
	list_remove(queue->entries, queue->size - 1);
	queue->size--;
	return data;
}
void *queue_peek(struct Queue *queue) { return queue->entries->head->data; }
