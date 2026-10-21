/*
 * Graph Traversal Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "test.h"
#include "tests.h"

#include "algorithms/traversal.h"

#include <limits.h>

// The nodes the visit callback saw, in the order it saw them.
static uint64_t visited[16];
static size_t visited_count;

// Traversal callback that records every node it is given.
static void collect(struct GraphNode *node, void *context) {
	(void)context;
	if (visited_count < 16) {
		visited[visited_count++] = node->data;
	}
}

// Clears the collection buffer before a traversal.
static void reset_visited(void) { visited_count = 0; }

// Builds a graph shaped like this:
//
//        1 --> 2 --> 3
//        |     |
//        v     v
//        4 --> 5
//
// The edges are added in this order: 1->2, 1->4, 2->3, 2->5, 4->5.
static struct Graph *build_graph(void) {
	struct Graph *graph = create_graph();

	add_node(graph, 1);
	add_node(graph, 2);
	add_node(graph, 3);
	add_node(graph, 4);
	add_node(graph, 5);

	add_edge(graph, graph->nodes[0], graph->nodes[1], 1);
	add_edge(graph, graph->nodes[0], graph->nodes[3], 1);
	add_edge(graph, graph->nodes[1], graph->nodes[2], 1);
	add_edge(graph, graph->nodes[1], graph->nodes[4], 1);
	add_edge(graph, graph->nodes[3], graph->nodes[4], 1);

	return graph;
}

// A breadth-first search visits every reachable node, level by level.
TEST(test_algo_bfs_visits_every_node) {
	struct Graph *graph = build_graph();

	reset_visited();
	size_t count = algo_bfs(graph, graph->nodes[0], collect, NULL);

	ASSERT_EQ_UINT(count, 5);
	ASSERT_EQ_UINT(visited_count, 5);

	// Level order: the start node, then its neighbours, then the rest.
	ASSERT_EQ_UINT(visited[0], 1);
	ASSERT_EQ_UINT(visited[1], 2);
	ASSERT_EQ_UINT(visited[2], 4);
	ASSERT_EQ_UINT(visited[3], 3);
	ASSERT_EQ_UINT(visited[4], 5);

	free_graph(graph);
}

// A breadth-first search does not leave the part of the graph it can reach.
TEST(test_algo_bfs_stays_within_the_reachable_part) {
	struct Graph *graph = build_graph();

	// Node 3 only has an edge coming in, so nothing is reachable from it.
	reset_visited();
	size_t count = algo_bfs(graph, graph->nodes[2], collect, NULL);

	ASSERT_EQ_UINT(count, 1);
	ASSERT_EQ_UINT(visited[0], 3);

	free_graph(graph);
}

// A depth-first search visits every reachable node as well.
TEST(test_algo_dfs_visits_every_node) {
	struct Graph *graph = build_graph();

	reset_visited();
	size_t count = algo_dfs(graph, graph->nodes[0], collect, NULL);

	ASSERT_EQ_UINT(count, 5);
	ASSERT_EQ_UINT(visited_count, 5);

	// The start node comes first and every node is visited exactly once.
	ASSERT_EQ_UINT(visited[0], 1);

	bool seen[6] = {false};
	for (size_t i = 0; i < visited_count; i++) {
		ASSERT(visited[i] >= 1 && visited[i] <= 5);
		ASSERT_FALSE(seen[visited[i]]);
		seen[visited[i]] = true;
	}

	free_graph(graph);
}

// The traversals reject invalid arguments instead of crashing.
TEST(test_algo_traversals_reject_invalid_arguments) {
	struct Graph *graph = build_graph();

	ASSERT_EQ_UINT(algo_bfs(NULL, graph->nodes[0], collect, NULL), 0);
	ASSERT_EQ_UINT(algo_bfs(graph, NULL, collect, NULL), 0);
	ASSERT_EQ_UINT(algo_bfs(graph, graph->nodes[0], NULL, NULL), 0);
	ASSERT_EQ_UINT(algo_dfs(NULL, graph->nodes[0], collect, NULL), 0);
	ASSERT_EQ_UINT(algo_dfs(graph, NULL, collect, NULL), 0);
	ASSERT_EQ_UINT(algo_dfs(graph, graph->nodes[0], NULL, NULL), 0);

	free_graph(graph);
}

// The shortest path is the one with the fewest edges.
TEST(test_algo_bfs_shortest_path) {
	struct Graph *graph = build_graph();
	struct GraphNode *path[8];
	size_t length = 0;

	ASSERT(algo_bfs_shortest_path(graph, graph->nodes[0], graph->nodes[4],
								   path, &length));

	ASSERT_EQ_UINT(length, 3);
	ASSERT_PTR_EQ(path[0], graph->nodes[0]);
	ASSERT_PTR_EQ(path[1], graph->nodes[1]);
	ASSERT_PTR_EQ(path[2], graph->nodes[4]);

	free_graph(graph);
}

// A node cannot reach itself through a path of zero edges unless it is the
// start node.
TEST(test_algo_bfs_shortest_path_to_the_start_node) {
	struct Graph *graph = build_graph();
	struct GraphNode *path[8];
	size_t length = 0;

	ASSERT(algo_bfs_shortest_path(graph, graph->nodes[0], graph->nodes[0],
								   path, &length));
	ASSERT_EQ_UINT(length, 1);
	ASSERT_PTR_EQ(path[0], graph->nodes[0]);

	free_graph(graph);
}

// There is no path when the goal cannot be reached.
TEST(test_algo_bfs_shortest_path_without_a_path) {
	struct Graph *graph = build_graph();
	struct GraphNode *path[8];
	size_t length = 0;

	// Node 3 has no outgoing edges, so it cannot reach node 1.
	ASSERT_FALSE(algo_bfs_shortest_path(graph, graph->nodes[2], graph->nodes[0],
										path, &length));

	free_graph(graph);
}

// Dijkstra's algorithm finds the cheapest distance to every node.
TEST(test_algo_dijkstra) {
	struct Graph *graph = create_graph();

	add_node(graph, 1);
	add_node(graph, 2);
	add_node(graph, 3);
	add_node(graph, 4);

	add_edge(graph, graph->nodes[0], graph->nodes[1], 10);
	add_edge(graph, graph->nodes[0], graph->nodes[2], 3);
	add_edge(graph, graph->nodes[2], graph->nodes[1], 4);
	add_edge(graph, graph->nodes[1], graph->nodes[3], 2);

	int dist[4];
	ASSERT(algo_dijkstra(graph, graph->nodes[0], dist));

	ASSERT_EQ_INT(dist[0], 0);
	ASSERT_EQ_INT(dist[1], 7);
	ASSERT_EQ_INT(dist[2], 3);
	ASSERT_EQ_INT(dist[3], 9);

	free_graph(graph);
}

// A node that cannot be reached gets the maximum distance.
TEST(test_algo_dijkstra_reports_unreachable_nodes) {
	struct Graph *graph = create_graph();

	add_node(graph, 1);
	add_node(graph, 2);

	add_edge(graph, graph->nodes[0], graph->nodes[1], 5);

	int dist[2];
	ASSERT(algo_dijkstra(graph, graph->nodes[0], dist));

	ASSERT_EQ_INT(dist[0], 0);
	ASSERT_EQ_INT(dist[1], 5);

	// Starting from the other node leaves the first one unreachable.
	ASSERT(algo_dijkstra(graph, graph->nodes[1], dist));
	ASSERT_EQ_INT(dist[0], INT_MAX);
	ASSERT_EQ_INT(dist[1], 0);

	free_graph(graph);
}

// Dijkstra's algorithm needs edge weights that are zero or positive.
TEST(test_algo_dijkstra_rejects_negative_weights) {
	struct Graph *graph = create_graph();

	add_node(graph, 1);
	add_node(graph, 2);
	add_edge(graph, graph->nodes[0], graph->nodes[1], -5);

	int dist[2];
	ASSERT_FALSE(algo_dijkstra(graph, graph->nodes[0], dist));

	free_graph(graph);
}

// The distance algorithm rejects invalid arguments instead of crashing.
TEST(test_algo_dijkstra_rejects_invalid_arguments) {
	struct Graph *graph = build_graph();
	int dist[5];

	ASSERT_FALSE(algo_dijkstra(NULL, graph->nodes[0], dist));
	ASSERT_FALSE(algo_dijkstra(graph, NULL, dist));
	ASSERT_FALSE(algo_dijkstra(graph, graph->nodes[0], NULL));

	free_graph(graph);
}

// Runs every graph traversal test case.
void run_traversal_tests(void) {
	test_section("traversal");

	RUN_TEST(test_algo_bfs_visits_every_node);
	RUN_TEST(test_algo_bfs_stays_within_the_reachable_part);
	RUN_TEST(test_algo_dfs_visits_every_node);
	RUN_TEST(test_algo_traversals_reject_invalid_arguments);
	RUN_TEST(test_algo_bfs_shortest_path);
	RUN_TEST(test_algo_bfs_shortest_path_to_the_start_node);
	RUN_TEST(test_algo_bfs_shortest_path_without_a_path);
	RUN_TEST(test_algo_dijkstra);
	RUN_TEST(test_algo_dijkstra_reports_unreachable_nodes);
	RUN_TEST(test_algo_dijkstra_rejects_negative_weights);
	RUN_TEST(test_algo_dijkstra_rejects_invalid_arguments);
}
