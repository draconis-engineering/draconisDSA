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

#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include <stdbool.h>
#include <stddef.h>

#include "graph/graph.h"

/**
 * @brief Callback that a traversal calls for every node it reaches.
 *
 * @param node The node that was reached.
 * @param context The pointer that was handed to the traversal.
 */
typedef void (*algo_visit_fn)(struct GraphNode *node, void *context);

/**
 * @brief Walk the graph breadth first, starting at `start`.
 *
 * Breadth-first search visits every node on the current level before it
 * moves on to the next one, which makes it the natural way to find the
 * shortest path in terms of number of edges.
 *
 * @param graph The graph to walk. May be NULL.
 * @param start The node to start from. Must be part of the graph.
 * @param visit Called once for every node that is reached. Must not be NULL.
 * @param context Passed on to `visit`, may be NULL.
 * @return The number of nodes that were visited.
 */
size_t algo_bfs(struct Graph *graph, struct GraphNode *start,
				algo_visit_fn visit, void *context);

/**
 * @brief Walk the graph depth first, starting at `start`.
 *
 * Depth-first search follows one branch as far as it goes before it backtracks
 * and tries the next one. The implementation keeps its own stack instead of
 * recursing, so it cannot run out of stack space on a deep graph.
 *
 * @param graph The graph to walk. May be NULL.
 * @param start The node to start from. Must be part of the graph.
 * @param visit Called once for every node that is reached. Must not be NULL.
 * @param context Passed on to `visit`, may be NULL.
 * @return The number of nodes that were visited.
 */
size_t algo_dfs(struct Graph *graph, struct GraphNode *start,
				algo_visit_fn visit, void *context);

/**
 * @brief Find the path with the fewest edges from `start` to `goal`.
 *
 * The path is written into `out_path`, which has to be able to hold one
 * pointer per node in the graph.
 *
 * @param graph The graph to search. May be NULL.
 * @param start The node to start from. Must be part of the graph.
 * @param goal The node to reach. Must be part of the graph.
 * @param out_path Buffer that receives the nodes of the path, in order.
 * @param out_length Receives the number of nodes in the path.
 * @return True if a path was found, false otherwise.
 */
bool algo_bfs_shortest_path(struct Graph *graph, struct GraphNode *start,
							 struct GraphNode *goal, struct GraphNode **out_path,
							 size_t *out_length);

/**
 * @brief Compute the cheapest path from `start` to every other node.
 *
 * This is Dijkstra's algorithm, which finds the shortest paths in a graph
 * whose edge weights are all zero or positive. Every edge of the graph has
 * to have a non-negative weight.
 *
 * The distances are written into `out_dist`, which has to be able to hold
 * one value per node in the graph. `out_dist[i]` receives the distance to
 * `graph->nodes[i]`. A node that cannot be reached gets the value INT_MAX.
 *
 * @param graph The graph to search. May be NULL.
 * @param start The node to start from. Must be part of the graph.
 * @param out_dist Buffer that receives the distance to every node.
 * @return True if the distances were computed, false on invalid input or
 *         allocation failure.
 */
bool algo_dijkstra(struct Graph *graph, struct GraphNode *start, int *out_dist);

#endif // TRAVERSAL_H
