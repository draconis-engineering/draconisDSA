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

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stdint.h>

struct GraphNode {
	uint64_t data;
	struct GraphNode *next;
};

struct GraphEdge {
	struct GraphNode *src;
	struct GraphNode *dest;
	int weight;
};

struct Graph {
	struct GraphNode **nodes;
	struct GraphEdge **edges;
	int num_nodes;
	int num_edges;
};

struct Graph *create_graph();

void add_node(struct Graph *graph, uint64_t data);
void add_edge(struct Graph *graph, struct GraphNode *src,
			  struct GraphNode *dest, int weight);

void remove_node(struct Graph *graph, struct GraphNode *node);
void remove_edge(struct Graph *graph, struct GraphEdge *edge);

void print_graph(struct Graph *graph);
void free_graph(struct Graph *graph);

bool has_node(struct Graph *graph, struct GraphNode *node);
bool has_edge(struct Graph *graph, struct GraphNode *src,
			  struct GraphNode *dest);
bool path_exists(struct Graph *graph, struct GraphNode *src,
				 struct GraphNode *dest);
bool connected(struct Graph *graph);

#endif // GRAPH_H
