/*
 * Demo Program - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "algorithms/algorithms.h"
#include "array/array.h"
#include "dllist/dllist.h"
#include "graph/graph.h"
#include "hashmap/hashmap.h"
#include "hashset/hashset.h"
#include "heap/heap.h"
#include "queue/queue.h"
#include "sllist/sllist.h"
#include "stack/stack.h"
#include "trees/trees.h"

#include <inttypes.h>
#include <stdio.h>

// Prints a payload that is expected to hold a string.
static void print_val(void *val) { printf("%s ", (char *)val); }

// Orders two integers ascending.
static int cmp_int(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x > y) - (x < y);
}

// Prints the data of a node that a graph traversal reaches.
static void print_node(struct GraphNode *node, void *context) {
	(void)context;
	printf("%" PRIu64 " ", node->data);
}

// Prints a short heading so the output is easy to follow.
static void heading(const char *title) { printf("\n== %s ==\n", title); }

int main() {
	// Dynamic array
	heading("Dynamic array");
	struct Array *array = create_array();
	int arr_vals[] = {1, 2, 3, 4};
	arr_push(array, &arr_vals[0]);
	arr_push(array, &arr_vals[1]);
	arr_push(array, &arr_vals[2]);
	arr_insert(array, &arr_vals[3], 1);
	printf("size: %d, element 2: %d\n", arr_size(array),
		   *(int *)arr_get(array, 2));
	arr_pop(array);
	arr_remove(array, 0);
	printf("size after pop and remove: %d\n", arr_size(array));
	free_array(array);

	// Singly linked list
	heading("Singly linked list");
	struct SLList *sllist = create_sll();
	int sll_vals[] = {1, 2, 3, 4};
	sll_insert_at_back(sllist, &sll_vals[0]);
	sll_insert_at_front(sllist, &sll_vals[1]);
	sll_insert_at_back(sllist, &sll_vals[2]);
	sll_insert(sllist, &sll_vals[3], 2);
	printf("size: %d\n", sll_size(sllist));
	sll_remove(sllist, 1);
	printf("size after remove: %d\n", sll_size(sllist));
	free_sll(sllist);

	// Doubly linked list
	heading("Doubly linked list");
	struct DLList *dllist = create_dll();
	int dll_vals[] = {10, 20, 30};
	dll_insert_at_back(dllist, &dll_vals[0]);
	dll_insert_at_front(dllist, &dll_vals[1]);
	dll_insert(dllist, &dll_vals[2], 1);
	printf("size: %d\n", dll_size(dllist));
	dll_remove(dllist, 0);
	printf("size after remove: %d\n", dll_size(dllist));
	free_dll(dllist);

	// Stack (LIFO)
	heading("Stack (LIFO)");
	struct Stack *stack = create_stack();
	stack_push(stack, (void *)"bottom");
	stack_push(stack, (void *)"top");
	printf("peek: %s\n", (char *)stack_peek(stack));
	printf("pop: %s\n", (char *)stack_pop(stack));
	free_stack(stack);

	// Queue (FIFO)
	heading("Queue (FIFO)");
	struct Queue *queue = create_queue();
	queue_add(queue, (void *)"first");
	queue_add(queue, (void *)"second");
	printf("peek: %s\n", (char *)queue_peek(queue));
	printf("get: %s\n", (char *)queue_get(queue));
	printf("get: %s\n", (char *)queue_get(queue));
	free_queue(queue);

	// Binary heap
	heading("Binary heap");
	struct Heap *heap = create_heap(cmp_int);
	int heap_vals[] = {5, 1, 8, 3};
	for (int i = 0; i < 4; i++) {
		heap_push(heap, &heap_vals[i]);
	}
	printf("smallest first: ");
	while (!heap_is_empty(heap)) {
		printf("%d ", *(int *)heap_pop(heap));
	}
	printf("\n");
	free_heap(heap);

	// Hash map
	heading("Hash map");
	struct HashMap *map = create_hashmap();
	map_insert(map, "draconis", 1);
	map_insert(map, "github", 2);
	map_insert(map, "draconis", 3);
	int found = 0;
	printf("draconis -> %d\n", map_get(map, "draconis", &found));
	map_remove(map, "github");
	found = 0;
	map_get(map, "github", &found);
	printf("github found after remove: %s\n", found ? "yes" : "no");
	free_hashmap(map);

	// Hash set
	heading("Hash set");
	struct HashSet *set = create_hashset();
	set_add(set, "a");
	set_add(set, "b");
	set_add(set, "c");
	set_remove(set, "b");
	printf("contains a: %s\n", set_contains(set, "a") ? "yes" : "no");
	free_hashset(set);

	// Graph
	heading("Graph");
	struct Graph *graph = create_graph();
	add_node(graph, 1);
	add_node(graph, 2);
	add_node(graph, 3);
	add_edge(graph, graph->nodes[0], graph->nodes[1], 5);
	add_edge(graph, graph->nodes[1], graph->nodes[2], 2);
	printf("path 1 -> 3: %s\n",
		   path_exists(graph, graph->nodes[0], graph->nodes[2]) ? "yes" : "no");
	free_graph(graph);

	// Binary tree
	heading("Binary tree");
	struct BinaryTree *broot = create_btree((void *)"root");
	struct BinaryTree *left = btree_insert_left(broot, (void *)"left");
	btree_insert_right(broot, (void *)"right");
	btree_insert_left(left, (void *)"left-left");
	btree_insert_right(left, (void *)"left-right");
	printf("size: %d, height: %d\n", btree_size(broot), btree_height(broot));
	printf("preorder:  ");
	btree_traverse_preorder(broot, print_val);
	printf("\ninorder:   ");
	btree_traverse_inorder(broot, print_val);
	printf("\npostorder: ");
	btree_traverse_postorder(broot, print_val);
	printf("\n");
	btree_free(broot);

	// N-ary tree
	heading("N-ary tree");
	struct Tree *troot = create_tree((void *)"root");
	tree_add_child(troot, (void *)"a");
	struct Tree *b = tree_add_child(troot, (void *)"b");
	tree_add_child(b, (void *)"b1");
	tree_add_child(b, (void *)"b2");
	printf("root children: %d, b children: %d\n", tree_num_children(troot),
		   tree_num_children(b));
	tree_free(troot);

	// Sorting
	heading("Sorting");
	int to_sort[] = {5, 3, 9, 1, 7};
	void *items[5];
	for (int i = 0; i < 5; i++) {
		items[i] = &to_sort[i];
	}
	algo_quick_sort(items, 5, cmp_int);
	printf("quick sort:   ");
	for (int i = 0; i < 5; i++) {
		printf("%d ", *(int *)items[i]);
	}
	printf("\n");

	algo_merge_sort(items, 5, cmp_int);
	printf("merge sort:   ");
	for (int i = 0; i < 5; i++) {
		printf("%d ", *(int *)items[i]);
	}
	printf("\n");

	// Searching
	heading("Searching");
	int target = 7;
	printf("binary search for 7: index %ld\n",
		   algo_binary_search(items, 5, &target, cmp_int));
	printf("linear search for 7:  index %ld\n",
		   algo_linear_search(items, 5, &target, cmp_int));

	// Graph traversal
	heading("Graph traversal");
	struct Graph *walk = create_graph();
	add_node(walk, 1);
	add_node(walk, 2);
	add_node(walk, 3);
	add_node(walk, 4);
	add_edge(walk, walk->nodes[0], walk->nodes[1], 1);
	add_edge(walk, walk->nodes[0], walk->nodes[2], 1);
	add_edge(walk, walk->nodes[1], walk->nodes[3], 1);

	printf("breadth first: ");
	algo_bfs(walk, walk->nodes[0], print_node, NULL);
	printf("\ndepth first:   ");
	algo_dfs(walk, walk->nodes[0], print_node, NULL);
	printf("\n");

	struct GraphNode *path[4];
	size_t path_length = 0;
	if (algo_bfs_shortest_path(walk, walk->nodes[0], walk->nodes[3], path,
							   &path_length)) {
		printf("shortest path 1 -> 4: ");
		for (size_t i = 0; i < path_length; i++) {
			printf("%" PRIu64 " ", path[i]->data);
		}
		printf("\n");
	}

	int distances[4];
	algo_dijkstra(walk, walk->nodes[0], distances);
	printf("cheapest distance from 1: ");
	for (int i = 0; i < 4; i++) {
		printf("%" PRIu64 "=%d ", walk->nodes[i]->data, distances[i]);
	}
	printf("\n");

	free_graph(walk);

	return 0;
}
