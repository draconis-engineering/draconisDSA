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

/**
 * @brief A node (vertex) in the graph.
 */
struct GraphNode {
	/** Data stored in the node. */
	uint64_t data;
	/**
	 * Position of the node in the graph's node array.
	 *
	 * The index is kept up to date by the graph itself, which lets the
	 * algorithms in `src/algorithms` look up a node's slot in O(1) instead
	 * of searching the node array.
	 */
	int index;
};

/**
 * @brief An edge between two nodes in the graph.
 */
struct GraphEdge {
	/** Source node of the edge. */
	struct GraphNode *src;
	/** Destination node of the edge. */
	struct GraphNode *dest;
	/** Weight of the edge. */
	int weight;
};

/**
 * @brief A graph representation storing arrays of nodes and edges.
 */
struct Graph {
	/** Array of pointers to graph nodes. */
	struct GraphNode **nodes;
	/** Array of pointers to graph edges. */
	struct GraphEdge **edges;
	/** Number of nodes in the graph. */
	int num_nodes;
	/** Number of edges in the graph. */
	int num_edges;
};

/**
 * @brief Create and initialize an empty graph.
 * @return A pointer to the new graph, or NULL on allocation failure.
 */
struct Graph *create_graph();

/**
 * @brief Add a new node to the graph.
 * @param graph Pointer to the graph.
 * @param data Data to store in the node.
 */
void add_node(struct Graph *graph, uint64_t data);

/**
 * @brief Add an edge between two nodes.
 * @param graph Pointer to the graph.
 * @param src Source node.
 * @param dest Destination node.
 * @param weight Weight of the edge.
 */
void add_edge(struct Graph *graph, struct GraphNode *src,
			  struct GraphNode *dest, int weight);

/**
 * @brief Remove a node from the graph and all incident edges.
 * @param graph Pointer to the graph.
 * @param node Node to remove.
 */
void remove_node(struct Graph *graph, struct GraphNode *node);

/**
 * @brief Remove an edge from the graph.
 * @param graph Pointer to the graph.
 * @param edge Edge to remove.
 */
void remove_edge(struct Graph *graph, struct GraphEdge *edge);

/**
 * @brief Print the graph nodes as data values.
 * @param graph Pointer to the graph.
 */
void print_graph(struct Graph *graph);

/**
 * @brief Free all memory used by the graph.
 * @param graph Pointer to the graph.
 */
void free_graph(struct Graph *graph);

/**
 * @brief Check if the graph contains a specific node.
 * @param graph Pointer to the graph.
 * @param node Node to look for.
 * @return True if the node exists, false otherwise.
 */
bool has_node(struct Graph *graph, struct GraphNode *node);

/**
 * @brief Check if there is an edge from src to dest.
 * @param graph Pointer to the graph.
 * @param src Source node.
 * @param dest Destination node.
 * @return True if the edge exists, false otherwise.
 */
bool has_edge(struct Graph *graph, struct GraphNode *src,
			  struct GraphNode *dest);

/**
 * @brief Check if a path exists from src to dest using BFS.
 * @param graph Pointer to the graph.
 * @param src Source node.
 * @param dest Destination node.
 * @return True if a path exists, false otherwise.
 */
bool path_exists(struct Graph *graph, struct GraphNode *src,
				 struct GraphNode *dest);

/**
 * @brief Check if the graph is strongly connected (all-pairs reachability).
 * @param graph Pointer to the graph.
 * @return True if strongly connected, false otherwise.
 */
bool connected(struct Graph *graph);

#endif // GRAPH_H
