/*
 * Graph Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "graph/graph.h"

// A new graph has no nodes and no edges.
TEST(test_graph_create_is_empty) {
	struct Graph *g = create_graph();

	ASSERT_NOT_NULL(g);
	ASSERT_EQ_INT(g->num_nodes, 0);
	ASSERT_EQ_INT(g->num_edges, 0);

	free_graph(g);
}

// Nodes are appended and can be looked up again.
TEST(test_graph_add_node) {
	struct Graph *g = create_graph();

	add_node(g, 1);
	add_node(g, 2);
	add_node(g, 3);

	ASSERT_EQ_INT(g->num_nodes, 3);
	ASSERT_EQ_UINT(g->nodes[0]->data, 1);
	ASSERT_EQ_UINT(g->nodes[1]->data, 2);
	ASSERT_EQ_UINT(g->nodes[2]->data, 3);

	ASSERT(has_node(g, g->nodes[0]));
	ASSERT(has_node(g, g->nodes[2]));

	free_graph(g);
}

// Edges connect two nodes that are part of the graph.
TEST(test_graph_add_edge) {
	struct Graph *g = create_graph();

	add_node(g, 1);
	add_node(g, 2);
	add_edge(g, g->nodes[0], g->nodes[1], 5);

	ASSERT_EQ_INT(g->num_edges, 1);
	ASSERT(has_edge(g, g->nodes[0], g->nodes[1]));
	ASSERT_FALSE(has_edge(g, g->nodes[1], g->nodes[0]));

	free_graph(g);
}

// An edge between nodes that are not in the graph is rejected.
TEST(test_graph_add_edge_with_unknown_node_is_ignored) {
	struct Graph *g = create_graph();
	struct Graph *other = create_graph();

	add_node(g, 1);
	add_node(other, 2);

	add_edge(g, g->nodes[0], other->nodes[0], 1);
	ASSERT_EQ_INT(g->num_edges, 0);

	free_graph(other);
	free_graph(g);
}

// A path exists along a chain of edges.
TEST(test_graph_path_exists) {
	struct Graph *g = create_graph();

	add_node(g, 1);
	add_node(g, 2);
	add_node(g, 3);
	add_edge(g, g->nodes[0], g->nodes[1], 1);
	add_edge(g, g->nodes[1], g->nodes[2], 1);

	ASSERT(path_exists(g, g->nodes[0], g->nodes[2]));
	ASSERT(path_exists(g, g->nodes[0], g->nodes[0]));
	ASSERT_FALSE(path_exists(g, g->nodes[2], g->nodes[0]));

	free_graph(g);
}

// Two nodes without a connection have no path between them.
TEST(test_graph_path_does_not_exist) {
	struct Graph *g = create_graph();

	add_node(g, 1);
	add_node(g, 2);

	ASSERT_FALSE(path_exists(g, g->nodes[0], g->nodes[1]));
	ASSERT_FALSE(path_exists(g, g->nodes[1], g->nodes[0]));

	free_graph(g);
}

// Removing a node also removes every edge that touched it.
TEST(test_graph_remove_node) {
	struct Graph *g = create_graph();

	add_node(g, 1);
	add_node(g, 2);
	add_node(g, 3);
	add_edge(g, g->nodes[0], g->nodes[1], 1);
	add_edge(g, g->nodes[1], g->nodes[2], 1);

	// Keep the address around so the removal can be checked afterwards. The
	// node itself is freed, so it is never dereferenced again.
	struct GraphNode *removed = g->nodes[1];
	remove_node(g, removed);

	ASSERT_EQ_INT(g->num_nodes, 2);
	ASSERT_EQ_INT(g->num_edges, 0);
	ASSERT_FALSE(has_node(g, removed));

	// The surviving nodes keep their data and close the gap.
	ASSERT(has_node(g, g->nodes[0]));
	ASSERT(has_node(g, g->nodes[1]));
	ASSERT_EQ_UINT(g->nodes[0]->data, 1);
	ASSERT_EQ_UINT(g->nodes[1]->data, 3);

	free_graph(g);
}

// Removing an edge leaves the nodes in place.
TEST(test_graph_remove_edge) {
	struct Graph *g = create_graph();

	add_node(g, 1);
	add_node(g, 2);
	add_edge(g, g->nodes[0], g->nodes[1], 1);
	ASSERT_EQ_INT(g->num_edges, 1);

	remove_edge(g, g->edges[0]);

	ASSERT_EQ_INT(g->num_edges, 0);
	ASSERT_EQ_INT(g->num_nodes, 2);
	ASSERT_FALSE(has_edge(g, g->nodes[0], g->nodes[1]));

	free_graph(g);
}

// A graph where every node reaches every other node is connected.
TEST(test_graph_connected) {
	struct Graph *g = create_graph();

	add_node(g, 1);
	add_node(g, 2);
	add_edge(g, g->nodes[0], g->nodes[1], 1);
	add_edge(g, g->nodes[1], g->nodes[0], 1);

	ASSERT(connected(g));

	free_graph(g);
}

// A graph with a one-way link is not connected.
TEST(test_graph_not_connected) {
	struct Graph *g = create_graph();

	add_node(g, 1);
	add_node(g, 2);
	add_edge(g, g->nodes[0], g->nodes[1], 1);

	ASSERT_FALSE(connected(g));

	free_graph(g);
}

// Freeing is safe with a NULL pointer.
TEST(test_graph_free_is_null_safe) {
	free_graph(NULL);
}

// Runs every graph test case.
void run_graph_tests(void) {
	test_section("graph");

	RUN_TEST(test_graph_create_is_empty);
	RUN_TEST(test_graph_add_node);
	RUN_TEST(test_graph_add_edge);
	RUN_TEST(test_graph_add_edge_with_unknown_node_is_ignored);
	RUN_TEST(test_graph_path_exists);
	RUN_TEST(test_graph_path_does_not_exist);
	RUN_TEST(test_graph_remove_node);
	RUN_TEST(test_graph_remove_edge);
	RUN_TEST(test_graph_connected);
	RUN_TEST(test_graph_not_connected);
	RUN_TEST(test_graph_free_is_null_safe);
}
