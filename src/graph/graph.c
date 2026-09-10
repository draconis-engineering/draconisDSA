
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "graph.h"

struct Graph* create_graph(int num_nodes, int num_edges) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->num_nodes = num_nodes;
    graph->num_edges = num_edges;

    return graph;
}
void free_graph(struct Graph* graph) {
    free(graph);
}

void add_node(struct Graph* graph, int data) {
    graph->nodes = (struct Node**)realloc(graph->nodes, (graph->num_nodes + 1) * sizeof(struct Node*));
    graph->nodes[graph->num_nodes] = (struct Node*)malloc(sizeof(struct Node));
    graph->nodes[graph->num_nodes]->data = data;
    graph->num_nodes++;
}
void add_edge(struct Graph* graph, struct Node* src, struct Node* dest, int weight) {
    graph->edges = (struct Edge**)realloc(graph->edges, (graph->num_edges + 1) * sizeof(struct Edge*));
    graph->edges[graph->num_edges] = (struct Edge*)malloc(sizeof(struct Edge));
    graph->edges[graph->num_edges]->src = src;
    graph->edges[graph->num_edges]->dest = dest;
    graph->edges[graph->num_edges]->weight = weight;
    graph->num_edges++;
}
void print_graph(struct Graph* graph) {
    for (int i = 0; i < graph->num_nodes; i++) {
        printf("%d ", graph->nodes[i]->data);
    }
    printf("\n");
    for (int i = 0; i < graph->num_edges; i++) {
        printf("%d -> %d (weight: %d)\n", graph->edges[i]->src->data, graph->edges[i]->dest->data, graph->edges[i]->weight);
    }
}

bool has_node(struct Graph* graph, struct Node* node) {
    for (int i = 0; i < graph->num_nodes; i++) {
        if (graph->nodes[i] == node) {
            return true;
        }
    }
    return false;
}

bool has_edge(struct Graph* graph, struct Node* src, struct Node* dest) {
    for (int i = 0; i < graph->num_edges; i++) {
        if (graph->edges[i]->src == src && graph->edges[i]->dest == dest) {
            return true;
        }
    }
    return false;
}

bool path_exists(struct Graph* graph, struct Node* src, struct Node* dest) {
    if (!has_node(graph, src) || !has_node(graph, dest)) {
        return false;
    }
    if (!has_edge(graph, src, dest)) {
        return false;
    }
    return true;
}

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
