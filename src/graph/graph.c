/*
* Graph Library - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
* Copyright (C) 2026 Simon Stordal Amundgård
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see http://www.gnu.org/licenses.
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "graph.h"

// Initializes the graph.
struct Graph* create_graph() {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->num_nodes = 0;
    graph->num_edges = 0;
    graph->nodes = NULL;
    graph->edges = NULL;
    return graph;
}

// Frees the memory used by the graph.
void free_graph(struct Graph* graph) {
    free(graph->nodes);
    free(graph->edges);
    free(graph);
}

// Adds a node to the graph.
void add_node(struct Graph* graph, uint64_t data) {
    graph->nodes = (struct Node**)realloc(graph->nodes, (graph->num_nodes + 1) * sizeof(struct Node*));
    graph->nodes[graph->num_nodes] = (struct Node*)malloc(sizeof(struct Node));
    graph->nodes[graph->num_nodes]->data = data;
    graph->num_nodes++;
}

// Adds an edge to the graph.
void add_edge(struct Graph* graph, struct Node* src, struct Node* dest, int weight) {
    graph->edges = (struct Edge**)realloc(graph->edges, (graph->num_edges + 1) * sizeof(struct Edge*));
    graph->edges[graph->num_edges] = (struct Edge*)malloc(sizeof(struct Edge));
    graph->edges[graph->num_edges]->src = src;
    graph->edges[graph->num_edges]->dest = dest;
    graph->edges[graph->num_edges]->weight = weight;
    graph->num_edges++;
}

// Prints the graph.
void print_graph(struct Graph* graph) {
    for (int i = 0; i < graph->num_nodes; i++) {
        printf("%llu ", graph->nodes[i]->data);
    }
    printf("\n");
}

// Checks if the graph has a node.
bool has_node(struct Graph* graph, struct Node* node) {
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->nodes[i] == node) {
            return true;
        }
    }
    return false;
}

// Checks if the graph has an edge.
bool has_edge(struct Graph* graph, struct Node* src, struct Node* dest) {
    for (int i = 0; i < graph->num_edges; i++) {
        if (graph->edges[i]->src == src && graph->edges[i]->dest == dest) {
            return true;
        }
    }
    return false;
}

// Checks if a path exists between two nodes.
bool path_exists(struct Graph* graph, struct Node* src, struct Node* dest) {
    if (!has_node(graph, src) || !has_node(graph, dest)) {
        return false;
    }
    if (!has_edge(graph, src, dest)) {
        return false;
    }
    return true;
}

// Checks if the graph is connected.
bool connected(struct Graph* graph) {
    for (int i = 0; i < graph->num_nodes; i++) {
        for (int j = 0; j < graph->num_nodes; j++) {
            if (i != j && !has_edge(graph, graph->nodes[i], graph->nodes[j])) {
                return false;
            }
        }
    }
    return true;
}
