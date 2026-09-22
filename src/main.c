#include <stdio.h>
#include <stdlib.h>

#include "dllist/dllist.h"
#include "graph/graph.h"
#include "hashmap/hashmap.h"
#include "queue/queue.h"
#include "sllist/sllist.h"
#include "stack/stack.h"
#include "trees/trees.h"

static void print_val(void *val) { printf("%s ", (char *)val); }

int main() {
	// Singly linked list
	struct SLList *sllist = create_sllist();
	sll_insert_at_back(sllist, (void *)1);
	sll_insert_at_front(sllist, (void *)2);
	sll_insert_at_back(sllist, (void *)3);
	sll_insert(sllist, (void *)4, 2);
	sll_remove(sllist, 1);
	print_sllist(sllist);
	free_sllist(sllist);

	// Doubly linked list
	struct DLList *dllist = create_dllist();
	dll_insert_at_back(dllist, (void *)10);
	dll_insert_at_front(dllist, (void *)20);
	dll_insert(dllist, (void *)30, 1);
	dll_remove(dllist, 0);
	print_list(dllist);
	free_list(dllist);

	// Stack (LIFO)
	struct Stack *stack = create_stack();
	stack_push(stack, (void *)"bottom");
	stack_push(stack, (void *)"top");
	printf("stack peek: %s\n", (char *)stack_peek(stack));
	printf("stack pop: %s\n", (char *)stack_pop(stack));
	free_stack(stack);

	// Queue (FIFO)
	struct Queue *queue = create_queue();
	queue_add(queue, (void *)"first");
	queue_add(queue, (void *)"second");
	printf("queue peek: %s\n", (char *)queue_peek(queue));
	printf("queue get: %s\n", (char *)queue_get(queue));
	printf("queue get: %s\n", (char *)queue_get(queue));
	free_queue(queue);

	// Hash map
	struct HashMap *map = create_hashmap();
	map_insert(map, "draconis", 1);
	map_insert(map, "github", 2);
	map_insert(map, "draconis", 3);
	int found = 0;
	printf("map draconis: %d\n", map_get(map, "draconis", &found));
	map_remove(map, "github");
	found = 0;
	map_get(map, "github", &found);
	printf("map github after remove found: %d\n", found);
	free_hashmap(map);

	// Graph
	struct Graph *graph = create_graph();
	add_node(graph, 1);
	add_node(graph, 2);
	add_node(graph, 3);
	add_edge(graph, graph->nodes[0], graph->nodes[1], 5);
	add_edge(graph, graph->nodes[1], graph->nodes[2], 2);
	printf("path 1 -> 3: %s\n",
		   path_exists(graph, graph->nodes[0], graph->nodes[2]) ? "yes" : "no");
	remove_node(graph, graph->nodes[1]);
	print_graph(graph);
	free_graph(graph);

	// Binary tree
	struct BinaryTree *broot = btree_create((void *)"root");
	struct BinaryTree *left = btree_insert_left(broot, (void *)"left");
	btree_insert_right(broot, (void *)"right");
	btree_insert_left(left, (void *)"left-left");
	btree_insert_right(left, (void *)"left-right");
	printf("btree size: %d, height: %d, parent of left: %s\n", btree_size(broot),
		   btree_height(broot), (char *)btree_get_val(btree_get_parent(left)));
	printf("preorder: ");
	btree_traverse_preorder(broot, print_val);
	printf("\ninorder:  ");
	btree_traverse_inorder(broot, print_val);
	printf("\npostorder:");
	btree_traverse_postorder(broot, print_val);
	printf("\n");
	btree_free(broot);

	// N-ary tree
	struct Tree *troot = tree_create((void *)"root");
	tree_add_child(troot, (void *)"a");
	struct Tree *b = tree_add_child(troot, (void *)"b");
	tree_add_child(b, (void *)"b1");
	tree_add_child(b, (void *)"b2");
	printf("n-ary root children: %d, b children: %d\n",
		   tree_num_children(troot), tree_num_children(b));
	tree_free(troot);

	return 0;
}
