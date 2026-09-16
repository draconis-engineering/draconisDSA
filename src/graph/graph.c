/*
 * Graph Library - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <inttypes.h>

#include "graph.h"

// Initializes the graph.
struct Graph *create_graph() {
	struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
	if (graph == NULL) {
		return NULL;
	}
	graph->num_nodes = 0;
	graph->num_edges = 0;
	graph->nodes = NULL;
	graph->edges = NULL;
	return graph;
}

// Frees all nodes, edges, and the graph itself.
void free_graph(struct Graph *graph) {
	for (int i = 0; i < graph->num_nodes; i++) {
		free(graph->nodes[i]);
	}
	for (int i = 0; i < graph->num_edges; i++) {
		free(graph->edges[i]);
	}
	free(graph->nodes);
	free(graph->edges);
	free(graph);
}

// Adds a node to the graph.
void add_node(struct Graph *graph, uint64_t data) {
	struct GraphNode *node = malloc(sizeof(struct GraphNode));
	if (node == NULL) {
		printf("Error: malloc failed\n");
		return;
	}
	node->data = data;
	node->next = NULL;

	graph->nodes = (struct GraphNode **)realloc(
		graph->nodes, (graph->num_nodes + 1) * sizeof(struct GraphNode *));
	if (graph->nodes == NULL) {
		free(node);
		printf("Error: realloc failed\n");
		return;
	}
	graph->nodes[graph->num_nodes] = node;
	graph->num_nodes++;
}

// Adds an edge to the graph.
void add_edge(struct Graph *graph, struct GraphNode *src,
			  struct GraphNode *dest, int weight) {
	if (!has_node(graph, src) || !has_node(graph, dest)) {
		return;
	}

	struct GraphEdge *edge = malloc(sizeof(struct GraphEdge));
	if (edge == NULL) {
		printf("Error: malloc failed\n");
		return;
	}
	edge->src = src;
	edge->dest = dest;
	edge->weight = weight;

	graph->edges = (struct GraphEdge **)realloc(
		graph->edges, (graph->num_edges + 1) * sizeof(struct GraphEdge *));
	if (graph->edges == NULL) {
		free(edge);
		printf("Error: realloc failed\n");
		return;
	}
	graph->edges[graph->num_edges] = edge;
	graph->num_edges++;
}

// Removes a node from the graph along with all incident edges.
void remove_node(struct Graph *graph, struct GraphNode *node) {
	bool found = false;
	for (int i = 0; i < graph->num_nodes; i++) {
		if (graph->nodes[i] == node) {
			free(graph->nodes[i]);
			for (int j = i; j < graph->num_nodes - 1; j++) {
				graph->nodes[j] = graph->nodes[j + 1];
			}
			graph->num_nodes--;
			found = true;
			break;
		}
	}

	if (!found) {
		return;
	}

	// Drop all edges incident to the node.
	int write = 0;
	for (int i = 0; i < graph->num_edges; i++) {
		if (graph->edges[i]->src == node || graph->edges[i]->dest == node) {
			free(graph->edges[i]);
			continue;
		}
		graph->edges[write++] = graph->edges[i];
	}
	graph->num_edges = write;
}

// Removes an edge from the graph.
void remove_edge(struct Graph *graph, struct GraphEdge *edge) {
	for (int i = 0; i < graph->num_edges; i++) {
		if (graph->edges[i] == edge) {
			free(graph->edges[i]);
			for (int j = i; j < graph->num_edges - 1; j++) {
				graph->edges[j] = graph->edges[j + 1];
			}
			graph->num_edges--;
			return;
		}
	}
}

// Prints the graph.
void print_graph(struct Graph *graph) {
	for (int i = 0; i < graph->num_nodes; i++) {
		printf("%" PRIu64 " ", graph->nodes[i]->data);
	}
	printf("\n");
}

// Checks if the graph has a node.
bool has_node(struct Graph *graph, struct GraphNode *node) {
	for (int i = 0; i < graph->num_nodes; i++) {
		if (graph->nodes[i] == node) {
			return true;
		}
	}
	return false;
}

// Checks if the graph has an edge.
bool has_edge(struct Graph *graph, struct GraphNode *src,
			  struct GraphNode *dest) {
	for (int i = 0; i < graph->num_edges; i++) {
		if (graph->edges[i]->src == src && graph->edges[i]->dest == dest) {
			return true;
		}
	}
	return false;
}

// Checks if a path exists between two nodes using breadth-first search.
bool path_exists(struct Graph *graph, struct GraphNode *src,
				 struct GraphNode *dest) {
	if (!has_node(graph, src) || !has_node(graph, dest)) {
		return false;
	}
	if (src == dest) {
		return true;
	}

	int n = graph->num_nodes;
	struct GraphNode **queue =
		(struct GraphNode **)malloc(n * sizeof(struct GraphNode *));
	bool *visited = (bool *)calloc(n, sizeof(bool));
	if (queue == NULL || visited == NULL) {
		free(queue);
		free(visited);
		return false;
	}

	int head = 0;
	int tail = 0;
	queue[tail++] = src;

	for (int i = 0; i < n; i++) {
		if (graph->nodes[i] == src) {
			visited[i] = true;
			break;
		}
	}

	while (head < tail) {
		struct GraphNode *curr = queue[head++];

		for (int e = 0; e < graph->num_edges; e++) {
			if (graph->edges[e]->src != curr) {
				continue;
			}

			struct GraphNode *next = graph->edges[e]->dest;
			if (next == dest) {
				free(queue);
				free(visited);
				return true;
			}

			for (int i = 0; i < n; i++) {
				if (graph->nodes[i] == next && !visited[i]) {
					visited[i] = true;
					queue[tail++] = next;
					break;
				}
			}
		}
	}

	free(queue);
	free(visited);
	return false;
}

// Checks whether the graph is strongly connected, i.e. every node is
// reachable from every other node.
bool connected(struct Graph *graph) {
	for (int i = 0; i < graph->num_nodes; i++) {
		for (int j = 0; j < graph->num_nodes; j++) {
			if (i != j &&
				!path_exists(graph, graph->nodes[i], graph->nodes[j])) {
				return false;
			}
		}
	}
	return true;
}
