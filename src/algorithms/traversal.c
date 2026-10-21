/*
 * Graph Traversal Algorithms - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "traversal.h"

#include "heap/heap.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Returns the position of a node in the graph's node array, or -1 if the node
// is not part of the graph.
static int node_index(const struct Graph *graph, const struct GraphNode *node) {
	if (graph == NULL || node == NULL) {
		return -1;
	}

	for (int i = 0; i < graph->num_nodes; i++) {
		if (graph->nodes[i] == node) {
			return i;
		}
	}

	return -1;
}

// Returns the position of a node in the graph's node array, or -1 if the
// index is out of range.
static int safe_index(const struct Graph *graph, int index) {
	if (index < 0 || index >= graph->num_nodes) {
		return -1;
	}
	return index;
}

/*
 * Breadth-first search: keep a queue of the nodes that still have to be
 * looked at, and always take the oldest one from the front.
 */
size_t algo_bfs(struct Graph *graph, struct GraphNode *start,
				algo_visit_fn visit, void *context) {
	if (graph == NULL || start == NULL || visit == NULL) {
		return 0;
	}

	int count_nodes = graph->num_nodes;
	int start_index = node_index(graph, start);
	if (count_nodes == 0 || start_index < 0) {
		return 0;
	}

	bool *visited = (bool *)calloc(count_nodes, sizeof(bool));
	int *queue = (int *)malloc(count_nodes * sizeof(int));
	if (visited == NULL || queue == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		free(visited);
		free(queue);
		return 0;
	}

	int head = 0;
	int tail = 0;
	visited[start_index] = true;
	queue[tail++] = start_index;

	size_t visited_count = 0;

	while (head < tail) {
		int index = queue[head++];
		struct GraphNode *node = graph->nodes[index];

		visit(node, context);
		visited_count++;

		for (int e = 0; e < graph->num_edges; e++) {
			struct GraphEdge *edge = graph->edges[e];
			if (edge->src != node) {
				continue;
			}

			int next = safe_index(graph, edge->dest->index);
			if (next < 0 || visited[next]) {
				continue;
			}

			visited[next] = true;
			queue[tail++] = next;
		}
	}

	free(visited);
	free(queue);
	return visited_count;
}

/*
 * Depth-first search: keep a stack of the nodes that still have to be looked
 * at, and always take the newest one from the top.
 */
size_t algo_dfs(struct Graph *graph, struct GraphNode *start,
				algo_visit_fn visit, void *context) {
	if (graph == NULL || start == NULL || visit == NULL) {
		return 0;
	}

	int count_nodes = graph->num_nodes;
	int start_index = node_index(graph, start);
	if (count_nodes == 0 || start_index < 0) {
		return 0;
	}

	bool *visited = (bool *)calloc(count_nodes, sizeof(bool));
	int *stack = (int *)malloc(count_nodes * sizeof(int));
	int *neighbours = (int *)malloc(count_nodes * sizeof(int));
	if (visited == NULL || stack == NULL || neighbours == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		free(visited);
		free(stack);
		free(neighbours);
		return 0;
	}

	int top = 0;
	visited[start_index] = true;
	stack[top++] = start_index;

	size_t visited_count = 0;

	while (top > 0) {
		int index = stack[--top];
		struct GraphNode *node = graph->nodes[index];

		visit(node, context);
		visited_count++;

		// Collect the neighbours that have not been seen yet.
		int found = 0;
		for (int e = 0; e < graph->num_edges; e++) {
			struct GraphEdge *edge = graph->edges[e];
			if (edge->src != node) {
				continue;
			}

			int next = safe_index(graph, edge->dest->index);
			if (next < 0 || visited[next]) {
				continue;
			}

			visited[next] = true;
			neighbours[found++] = next;
		}

		// Push them in reverse order, so the first neighbour is on top of
		// the stack and is therefore the next one to be visited.
		for (int i = found - 1; i >= 0; i--) {
			stack[top++] = neighbours[i];
		}
	}

	free(visited);
	free(stack);
	free(neighbours);
	return visited_count;
}

/*
 * Breadth-first search that also remembers how every node was reached, so
 * that the path to the goal can be rebuilt afterwards.
 */
bool algo_bfs_shortest_path(struct Graph *graph, struct GraphNode *start,
							 struct GraphNode *goal, struct GraphNode **out_path,
							 size_t *out_length) {
	if (graph == NULL || start == NULL || goal == NULL || out_path == NULL ||
		out_length == NULL) {
		return false;
	}

	int count_nodes = graph->num_nodes;
	int start_index = node_index(graph, start);
	int goal_index = node_index(graph, goal);
	if (count_nodes == 0 || start_index < 0 || goal_index < 0) {
		return false;
	}

	bool *visited = (bool *)calloc(count_nodes, sizeof(bool));
	int *parent = (int *)malloc(count_nodes * sizeof(int));
	int *queue = (int *)malloc(count_nodes * sizeof(int));
	if (visited == NULL || parent == NULL || queue == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		free(visited);
		free(parent);
		free(queue);
		return false;
	}

	for (int i = 0; i < count_nodes; i++) {
		parent[i] = -1;
	}

	int head = 0;
	int tail = 0;
	visited[start_index] = true;
	parent[start_index] = start_index;
	queue[tail++] = start_index;

	bool found = false;

	while (head < tail) {
		int index = queue[head++];

		if (index == goal_index) {
			found = true;
			break;
		}

		struct GraphNode *node = graph->nodes[index];

		for (int e = 0; e < graph->num_edges; e++) {
			struct GraphEdge *edge = graph->edges[e];
			if (edge->src != node) {
				continue;
			}

			int next = safe_index(graph, edge->dest->index);
			if (next < 0 || visited[next]) {
				continue;
			}

			visited[next] = true;
			parent[next] = index;
			queue[tail++] = next;
		}
	}

	if (!found) {
		free(visited);
		free(parent);
		free(queue);
		return false;
	}

	// Count the nodes on the path.
	size_t length = 0;
	int index = goal_index;
	for (;;) {
		length++;
		if (index == start_index) {
			break;
		}
		index = parent[index];
	}

	// Fill the buffer from the back, walking the parents backwards.
	index = goal_index;
	for (size_t i = length; i > 0; i--) {
		out_path[i - 1] = graph->nodes[index];
		if (index == start_index) {
			break;
		}
		index = parent[index];
	}

	*out_length = length;

	free(visited);
	free(parent);
	free(queue);
	return true;
}

// One entry of Dijkstra's priority queue.
struct DijkstraEntry {
	/** Position of the node in the graph's node array. */
	int index;
	/** Distance to the node when the entry was added. */
	int distance;
};

// Orders the priority queue by distance, smallest first.
static int dijkstra_compare(const void *a, const void *b) {
	const struct DijkstraEntry *x = (const struct DijkstraEntry *)a;
	const struct DijkstraEntry *y = (const struct DijkstraEntry *)b;
	return (x->distance > y->distance) - (x->distance < y->distance);
}

/*
 * Dijkstra's algorithm: always continue with the node that is currently
 * closest to the start, and relax the edges that lead out of it.
 */
bool algo_dijkstra(struct Graph *graph, struct GraphNode *start, int *out_dist) {
	if (graph == NULL || start == NULL || out_dist == NULL) {
		return false;
	}

	int count_nodes = graph->num_nodes;
	int start_index = node_index(graph, start);
	if (count_nodes == 0 || start_index < 0) {
		return false;
	}

	// Dijkstra's algorithm needs the edge weights to be zero or positive.
	for (int e = 0; e < graph->num_edges; e++) {
		if (graph->edges[e]->weight < 0) {
			fprintf(stderr,
					"Error: Dijkstra's algorithm needs non-negative weights\n");
			return false;
		}
	}

	int *dist = (int *)malloc(count_nodes * sizeof(int));
	if (dist == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return false;
	}

	for (int i = 0; i < count_nodes; i++) {
		dist[i] = INT_MAX;
	}
	dist[start_index] = 0;

	struct Heap *heap = create_heap(dijkstra_compare);
	if (heap == NULL) {
		free(dist);
		return false;
	}

	struct DijkstraEntry *first = (struct DijkstraEntry *)malloc(sizeof(*first));
	if (first == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		free(dist);
		free_heap(heap);
		return false;
	}
	first->index = start_index;
	first->distance = 0;
	heap_push(heap, first);

	while (!heap_is_empty(heap)) {
		struct DijkstraEntry *current =
			(struct DijkstraEntry *)heap_pop(heap);
		int index = current->index;
		int distance = current->distance;
		free(current);

		// The queue can hold an entry for a node that has already been
		// reached by a cheaper path. Such an entry is stale and skipped.
		if (distance != dist[index]) {
			continue;
		}

		struct GraphNode *node = graph->nodes[index];

		for (int e = 0; e < graph->num_edges; e++) {
			struct GraphEdge *edge = graph->edges[e];
			if (edge->src != node) {
				continue;
			}

			int next = safe_index(graph, edge->dest->index);
			if (next < 0) {
				continue;
			}

			// Guard against an overflow when the weights are huge.
			if (edge->weight > 0 && distance > INT_MAX - edge->weight) {
				continue;
			}

			int candidate = distance + edge->weight;
			if (candidate >= dist[next]) {
				continue;
			}

			dist[next] = candidate;

			struct DijkstraEntry *improved =
				(struct DijkstraEntry *)malloc(sizeof(*improved));
			if (improved == NULL) {
				fprintf(stderr, "Error: malloc failed\n");
				continue;
			}
			improved->index = next;
			improved->distance = candidate;
			heap_push(heap, improved);
		}
	}

	for (int i = 0; i < count_nodes; i++) {
		out_dist[i] = dist[i];
	}

	free(dist);
	free_heap(heap);
	return true;
}
