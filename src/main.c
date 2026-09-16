#include <stdio.h>
#include <stdlib.h>

#include "dllist/dllist.h"
#include "graph/graph.h"
#include "hashmap/hashmap.h"
#include "queue/queue.h"
#include "sllist/sllist.h"
#include "stack/stack.h"

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
	free(sllist);

	// Doubly linked list
	struct DLList *dllist = create_dllist();
	dll_insert_at_back(dllist, (void *)10);
	dll_insert_at_front(dllist, (void *)20);
	dll_insert(dllist, (void *)30, 1);
	dll_remove(dllist, 0);
	print_list(dllist);
	free_list(dllist);
	free(dllist);

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

	return 0;
}