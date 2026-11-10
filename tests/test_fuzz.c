/*
 * Fuzz Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * These tests feed random operation sequences to every container and check
 * the invariants after each operation. Every sequence is mirrored in a plain
 * "shadow model" (a C array), so the checks are really "the container still
 * agrees with the model", plus structural invariants such as list links and
 * the heap property.
 *
 * The generator is deterministic: the default seeds below always produce the
 * same runs, so a failure is reproducible. Set DRACONIS_FUZZ_SEED to a number
 * to run one different sequence, which is how new seeds are hunted.
 */

/** Operations per container per seed. */
#define FUZZ_OPS 300

/** The default seeds. Deterministic, so failures can be reproduced. */
static const uint64_t default_seeds[] = {0xD5AC0115, 0x5EEDF00D, 0xCA55E777};

/** Number of payloads in the pool the containers store pointers to. */
#define POOL_SIZE 256

/** The payloads. Storing pointers to these makes pointer equality work. */
static int pool[POOL_SIZE];

/** Starting state of the generator, advanced with splitmix64. */
static uint64_t rng_state = 0;

// ---------------------------------------------------------------------------
// The generator
// ---------------------------------------------------------------------------

// Orders plain ints ascending, the payload the heap fuzz uses.
static int cmp_int_fuzz(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x > y) - (x < y);
}

// Seeds the generator.
static void rng_seed(uint64_t seed) { rng_state = seed; }

// Returns the next random 64-bit number (splitmix64).
static uint64_t rng_next(void) {
	uint64_t z = (rng_state += 0x9E3779B97F4A7C15ULL);
	z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
	z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
	return z ^ (z >> 31);
}

// Returns a random number below `bound`. Requires bound > 0.
static uint32_t rng_below(uint32_t bound) { return (uint32_t)(rng_next() % bound); }

// Returns a random payload from the pool.
static void *rng_payload(void) { return &pool[rng_below(POOL_SIZE)]; }

// Fills the payload pool and returns the seeds to run.
// Reads DRACONIS_FUZZ_SEED once, so the tests stay deterministic within a run.
static void fuzz_setup(uint64_t *seeds, int *seed_count) {
	for (int i = 0; i < POOL_SIZE; i++) {
		pool[i] = i;
	}

	const char *override = getenv("DRACONIS_FUZZ_SEED");
	if (override != NULL && override[0] != '\0') {
		seeds[0] = strtoull(override, NULL, 0);
		*seed_count = 1;
		return;
	}

	for (int i = 0; i < (int)(sizeof(default_seeds) / sizeof(default_seeds[0])); i++) {
		seeds[i] = default_seeds[i];
	}
	*seed_count = (int)(sizeof(default_seeds) / sizeof(default_seeds[0]));
}

// ---------------------------------------------------------------------------
// Invariant checks
// ---------------------------------------------------------------------------

// Checks that the array still matches the shadow model element by element.
static void check_array(struct Array *a, void **model, int model_size) {
	ASSERT_EQ_INT(arr_size(a), model_size);
	ASSERT_EQ_INT(arr_is_empty(a), model_size == 0);
	ASSERT(a->size <= a->capacity);

	for (int i = 0; i < model_size; i++) {
		ASSERT_PTR_EQ(arr_get(a, i), model[i]);
	}
}

// Checks that the singly linked list still matches the shadow model, that
// walking it from the head visits exactly `model_size` nodes in order, and
// that the tail pointer points at the last node.
static void check_sll(struct SLList *l, void **model, int model_size) {
	ASSERT_EQ_INT(sll_size(l), model_size);
	ASSERT_EQ_INT(sll_is_empty(l), model_size == 0);

	struct SLLNode *node = l->head;
	int count = 0;
	while (node != NULL && count < model_size) {
		ASSERT_PTR_EQ(node->data, model[count]);
		count++;
		node = node->next;
	}
	ASSERT_EQ_INT(count, model_size);
	ASSERT_NULL(node);

	if (model_size == 0) {
		ASSERT_NULL(l->head);
		ASSERT_NULL(l->tail);
	} else {
		ASSERT_PTR_EQ(l->tail->data, model[model_size - 1]);
		ASSERT_NULL(l->tail->next);
	}
}

// Checks that the doubly linked list matches the model in both directions and
// that every prev pointer agrees with the forward walk.
static void check_dll(struct DLList *l, void **model, int model_size) {
	ASSERT_EQ_INT(dll_size(l), model_size);
	ASSERT_EQ_INT(dll_is_empty(l), model_size == 0);

	struct DLLNode *node = l->head;
	struct DLLNode *prev = NULL;
	int count = 0;
	while (node != NULL && count < model_size) {
		ASSERT_PTR_EQ(node->data, model[count]);
		ASSERT_PTR_EQ(node->prev, prev);
		prev = node;
		node = node->next;
		count++;
	}
	ASSERT_EQ_INT(count, model_size);
	ASSERT_NULL(node);

	if (model_size == 0) {
		ASSERT_NULL(l->head);
		ASSERT_NULL(l->tail);
	} else {
		ASSERT_PTR_EQ(l->tail, prev);
		ASSERT_NULL(l->tail->next);
		ASSERT_NULL(l->head->prev);
	}
}

// Checks that the heap still satisfies the heap property: no element may come
// before its parent in the comparator's order.
static void check_heap(struct Heap *heap, int *sorted_model, int model_size) {
	ASSERT_EQ_UINT(heap_size(heap), (size_t)model_size);
	ASSERT(heap_is_empty(heap) == (model_size == 0));

	for (size_t i = 1; i < heap->size; i++) {
		size_t parent = (i - 1) / 2;
		ASSERT(heap->cmp(heap->items[i], heap->items[parent]) >= 0);
	}

	if (model_size > 0) {
		// The top of the heap must be the smallest element of the model.
		ASSERT_EQ_INT(*(int *)heap_peek(heap), sorted_model[0]);
	}
}

// Counts the nodes of a binary tree through the public getters and checks
// that every child points back at its parent.
static int check_btree_node(struct BinaryTree *node, struct BinaryTree *parent,
							int seen_so_far) {
	if (node == NULL) {
		return seen_so_far;
	}

	ASSERT_PTR_EQ(btree_get_parent(node), parent);
	seen_so_far++;

	seen_so_far =
		check_btree_node(btree_get_left(node), node, seen_so_far);
	seen_so_far =
		check_btree_node(btree_get_right(node), node, seen_so_far);
	return seen_so_far;
}

// Computes reachability[i][j] for the shadow adjacency counts with a
// breadth-first search from every node.
static void shadow_reachability(int adjacency[][8], int node_count,
								bool reachability[][8]) {
	for (int i = 0; i < node_count; i++) {
		for (int j = 0; j < node_count; j++) {
			reachability[i][j] = false;
		}
		reachability[i][i] = true;
	}

	for (int start = 0; start < node_count; start++) {
		// A plain BFS over the shadow matrix.
		bool queued[8] = {false};
		int queue[8];
		int head = 0;
		int tail = 0;

		queued[start] = true;
		queue[tail++] = start;

		while (head < tail) {
			int current = queue[head++];
			for (int next = 0; next < node_count; next++) {
				if (adjacency[current][next] > 0 && !queued[next]) {
					queued[next] = true;
					reachability[start][next] = true;
					queue[tail++] = next;
				}
			}
		}
	}
}

// Checks the graph against the shadow model: edge counts, has_edge for every
// pair and path_exists for every pair of live nodes.
static void check_graph(struct Graph *graph, int adjacency[][8],
						int node_count) {
	ASSERT_EQ_INT(graph->num_nodes, node_count);

	// The shadow matrix has one row and column per live node, in the same
	// order as the graph's node array.
	int live_edges = 0;
	for (int i = 0; i < node_count; i++) {
		for (int j = 0; j < node_count; j++) {
			ASSERT(has_edge(graph, graph->nodes[i], graph->nodes[j]) ==
				   (adjacency[i][j] > 0));
			live_edges += adjacency[i][j];
		}
	}
	ASSERT_EQ_INT(graph->num_edges, live_edges);

	bool reachability[8][8];
	shadow_reachability(adjacency, node_count, reachability);

	for (int i = 0; i < node_count; i++) {
		for (int j = 0; j < node_count; j++) {
			ASSERT(path_exists(graph, graph->nodes[i], graph->nodes[j]) ==
				   reachability[i][j]);
		}
	}
}

// ---------------------------------------------------------------------------
// The fuzz tests
// ---------------------------------------------------------------------------

// Random pushes, inserts, sets, removals and pops on the dynamic array,
// checked against the shadow model after every operation.
TEST(test_fuzz_array_matches_model) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct Array *a = create_array();
		ASSERT_NOT_NULL(a);

		void *model[512];
		int model_size = 0;

		for (int op = 0; op < FUZZ_OPS; op++) {
			switch (rng_below(6)) {
			case 0:
			case 1: {
				if (model_size < 512) {
					void *val = rng_payload();
					arr_push(a, val);
					model[model_size++] = val;
				}
				break;
			}
			case 2: {
				if (model_size < 512) {
					int idx = (int)rng_below((uint32_t)model_size + 1);
					void *val = rng_payload();
					arr_insert(a, val, idx);
					for (int i = model_size; i > idx; i--) {
						model[i] = model[i - 1];
					}
					model[idx] = val;
					model_size++;
				}
				break;
			}
			case 3: {
				if (model_size > 0) {
					int idx = (int)rng_below((uint32_t)model_size);
					arr_remove(a, idx);
					for (int i = idx; i < model_size - 1; i++) {
						model[i] = model[i + 1];
					}
					model_size--;
				}
				break;
			}
			case 4: {
				if (model_size > 0) {
					int idx = (int)rng_below((uint32_t)model_size);
					void *val = rng_payload();
					arr_set(a, idx, val);
					model[idx] = val;
				}
				break;
			}
			default: {
				if (model_size > 0) {
					void *val = arr_pop(a);
					ASSERT_PTR_EQ(val, model[model_size - 1]);
					model_size--;
				}
				break;
			}
			}

			check_array(a, model, model_size);
		}

		free_array(a);
	}
}

// Random front, back and index insertions and removals on the singly linked
// list, checked against the shadow model after every operation.
TEST(test_fuzz_sll_matches_model) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct SLList *l = create_sll();
		ASSERT_NOT_NULL(l);

		void *model[512];
		int model_size = 0;

		for (int op = 0; op < FUZZ_OPS; op++) {
			switch (rng_below(4)) {
			case 0: {
				if (model_size < 512) {
					void *val = rng_payload();
					sll_insert_at_front(l, val);
					for (int i = model_size; i > 0; i--) {
						model[i] = model[i - 1];
					}
					model[0] = val;
					model_size++;
				}
				break;
			}
			case 1: {
				if (model_size < 512) {
					void *val = rng_payload();
					sll_insert_at_back(l, val);
					model[model_size++] = val;
				}
				break;
			}
			case 2: {
				if (model_size < 512) {
					// -1 is the documented "insert at the back" shortcut.
					int idx = (rng_below(4) == 0)
								  ? -1
								  : (int)rng_below((uint32_t)model_size + 1);
					void *val = rng_payload();
					sll_insert(l, val, idx);
					if (idx == -1) {
						idx = model_size;
					}
					for (int i = model_size; i > idx; i--) {
						model[i] = model[i - 1];
					}
					model[idx] = val;
					model_size++;
				}
				break;
			}
			default: {
				if (model_size > 0) {
					int idx = (int)rng_below((uint32_t)model_size);
					sll_remove(l, idx);
					for (int i = idx; i < model_size - 1; i++) {
						model[i] = model[i + 1];
					}
					model_size--;
				}
				break;
			}
			}

			check_sll(l, model, model_size);

			// Every now and then, look a payload up by pointer. With
			// duplicate pointers the lookup must return the first one.
			if (rng_below(8) == 0 && model_size > 0) {
				int pick = (int)rng_below((uint32_t)model_size);
				int first = 0;
				while (model[first] != model[pick]) {
					first++;
				}
				ASSERT_EQ_INT(sll_get_idx(l, model[pick]), first);
			}
		}

		free_sll(l);
	}
}

// Random insertions and removals on the doubly linked list, checked against
// the shadow model after every operation.
TEST(test_fuzz_dll_matches_model) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct DLList *l = create_dll();
		ASSERT_NOT_NULL(l);

		void *model[512];
		int model_size = 0;

		for (int op = 0; op < FUZZ_OPS; op++) {
			switch (rng_below(4)) {
			case 0: {
				if (model_size < 512) {
					void *val = rng_payload();
					dll_insert_at_front(l, val);
					for (int i = model_size; i > 0; i--) {
						model[i] = model[i - 1];
					}
					model[0] = val;
					model_size++;
				}
				break;
			}
			case 1: {
				if (model_size < 512) {
					void *val = rng_payload();
					dll_insert_at_back(l, val);
					model[model_size++] = val;
				}
				break;
			}
			case 2: {
				if (model_size < 512) {
					int idx = (rng_below(4) == 0)
								  ? -1
								  : (int)rng_below((uint32_t)model_size + 1);
					void *val = rng_payload();
					dll_insert(l, val, idx);
					if (idx == -1) {
						idx = model_size;
					}
					for (int i = model_size; i > idx; i--) {
						model[i] = model[i - 1];
					}
					model[idx] = val;
					model_size++;
				}
				break;
			}
			default: {
				if (model_size > 0) {
					int idx = (int)rng_below((uint32_t)model_size);
					dll_remove(l, idx);
					for (int i = idx; i < model_size - 1; i++) {
						model[i] = model[i + 1];
					}
					model_size--;
				}
				break;
			}
			}

			check_dll(l, model, model_size);

			if (rng_below(8) == 0 && model_size > 0) {
				int pick = (int)rng_below((uint32_t)model_size);
				int first = 0;
				while (model[first] != model[pick]) {
					first++;
				}
				ASSERT_EQ_INT(dll_get_idx(l, model[pick]), first);
			}
		}

		free_dll(l);
	}
}

// Random pushes and pops on the stack. The model is only touched at the top,
// so the invariant is that pops come back in the reverse order of the pushes.
TEST(test_fuzz_stack_is_lifo) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct Stack *stack = create_stack();
		ASSERT_NOT_NULL(stack);

		void *model[512];
		int model_size = 0;

		for (int op = 0; op < FUZZ_OPS; op++) {
			if (model_size == 0 || rng_below(3) != 0) {
				if (model_size < 512) {
					void *val = rng_payload();
					stack_push(stack, val);
					model[model_size++] = val;
				}
			} else {
				void *val = stack_pop(stack);
				ASSERT_PTR_EQ(val, model[model_size - 1]);
				model_size--;
			}

			if (model_size > 0) {
				ASSERT_PTR_EQ(stack_peek(stack), model[model_size - 1]);
			} else {
				ASSERT_NULL(stack_peek(stack));
				ASSERT_NULL(stack_pop(stack));
			}
		}

		free_stack(stack);
	}
}

// Random enqueues and dequeues on the queue. The invariant is that dequeues
// come back in the same order as the enqueues.
TEST(test_fuzz_queue_is_fifo) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct Queue *queue = create_queue();
		ASSERT_NOT_NULL(queue);

		void *model[512];
		int head = 0;
		int tail = 0;

		for (int op = 0; op < FUZZ_OPS; op++) {
			if (head == tail || rng_below(3) != 0) {
				if (tail < 512) {
					void *val = rng_payload();
					queue_add(queue, val);
					model[tail++] = val;
				}
			} else {
				void *val = queue_get(queue);
				ASSERT_PTR_EQ(val, model[head++]);
			}

			if (head < tail) {
				ASSERT_PTR_EQ(queue_peek(queue), model[head]);
			} else {
				ASSERT_NULL(queue_peek(queue));
				ASSERT_NULL(queue_get(queue));
			}
		}

		free_queue(queue);
	}
}

// Random inserts, updates, removals and lookups on the hash map, checked
// against a shadow array of keys after every operation.
TEST(test_fuzz_hashmap_matches_model) {
	char keys[64][16];
	for (int i = 0; i < 64; i++) {
		snprintf(keys[i], sizeof(keys[i]), "key%02d", i);
	}

	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct HashMap *map = create_hashmap();
		ASSERT_NOT_NULL(map);

		int values[64];
		bool present[64] = {false};

		for (int op = 0; op < FUZZ_OPS; op++) {
			int key = (int)rng_below(64);

			switch (rng_below(4)) {
			case 0:
			case 1: {
				int value = (int)rng_below(1000);
				map_insert(map, keys[key], value);
				values[key] = value;
				present[key] = true;
				break;
			}
			case 2: {
				if (present[key]) {
					map_remove(map, keys[key]);
					present[key] = false;
				}
				break;
			}
			default: {
				// Re-inserting a removed key must work.
				map_insert(map, keys[key], (int)key);
				values[key] = key;
				present[key] = true;
				break;
			}
			}

			// Every key must answer consistently.
			for (int k = 0; k < 64; k++) {
				int found = 0;
				int value = map_get(map, keys[k], &found);
				ASSERT_EQ_INT(found, present[k]);
				if (present[k]) {
					ASSERT_EQ_INT(value, values[k]);
				}
			}
		}

		free_hashmap(map);
	}
}

// Random adds, removes and lookups on the hash set, checked against a shadow
// array after every operation.
TEST(test_fuzz_hashset_matches_model) {
	char values[64][16];
	for (int i = 0; i < 64; i++) {
		snprintf(values[i], sizeof(values[i]), "value%02d", i);
	}

	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct HashSet *set = create_hashset();
		ASSERT_NOT_NULL(set);

		bool present[64] = {false};

		for (int op = 0; op < FUZZ_OPS; op++) {
			int value = (int)rng_below(64);

			switch (rng_below(3)) {
			case 0: {
				bool added = set_add(set, values[value]);
				ASSERT_EQ_INT(added, !present[value]);
				present[value] = true;
				break;
			}
			case 1: {
				if (present[value]) {
					ASSERT(set_remove(set, values[value]));
					present[value] = false;
				} else {
					ASSERT_FALSE(set_remove(set, values[value]));
				}
				break;
			}
			default: {
				ASSERT_EQ_INT(set_contains(set, values[value]),
							  present[value]);
				break;
			}
			}

			// The reported size must match the shadow count.
			int count = 0;
			for (int v = 0; v < 64; v++) {
				count += present[v];
			}
			ASSERT_EQ_UINT(set->size, (size_t)count);
		}

		free_hashset(set);
	}
}

// Random pushes and pops on the heap. The heap property is checked after
// every operation, and every pop must return the smallest live value.
TEST(test_fuzz_heap_pops_minimum) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct Heap *heap = create_heap(cmp_int_fuzz);
		ASSERT_NOT_NULL(heap);

		// The model is kept sorted, so model[0] is always the minimum.
		int model[512];
		int model_size = 0;

		for (int op = 0; op < FUZZ_OPS; op++) {
			if (model_size == 0 || rng_below(3) != 0) {
				if (model_size < 512) {
					int value = (int)rng_below(POOL_SIZE);
					heap_push(heap, &pool[value]);
					model[model_size++] = value;
					// Insertion sort step: keep the model sorted.
					int i = model_size - 1;
					while (i > 0 && model[i - 1] > model[i]) {
						int tmp = model[i - 1];
						model[i - 1] = model[i];
						model[i] = tmp;
						i--;
					}
				}
			} else {
				void *popped = heap_pop(heap);
				ASSERT_NOT_NULL(popped);
				ASSERT_EQ_INT(*(int *)popped, model[0]);
				for (int i = 1; i < model_size; i++) {
					model[i - 1] = model[i];
				}
				model_size--;
			}

			check_heap(heap, model, model_size);
		}

		free_heap(heap);
	}
}

// Random edge additions and removals on the graph, checked against a shadow
// adjacency matrix after every operation: has_edge and path_exists for every
// pair of nodes must agree with a BFS over the matrix.
TEST(test_fuzz_graph_matches_model) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct Graph *graph = create_graph();
		ASSERT_NOT_NULL(graph);

		const int node_count = 8;
		for (int i = 0; i < node_count; i++) {
			add_node(graph, (uint64_t)i);
		}

		// The shadow model: adjacency[i][j] counts edges from i to j.
		int adjacency[8][8] = {0};

		for (int op = 0; op < FUZZ_OPS; op++) {
			if (graph->num_edges == 0 || rng_below(3) != 0) {
				int src = (int)rng_below(node_count);
				int dest = (int)rng_below(node_count);
				add_edge(graph, graph->nodes[src], graph->nodes[dest],
						 (int)rng_below(10));
				adjacency[src][dest]++;
			} else {
				int which = (int)rng_below((uint32_t)graph->num_edges);
				// Find the shadow entry for the edge being removed.
				struct GraphEdge *edge = graph->edges[which];
				int src = -1;
				int dest = -1;
				for (int i = 0; i < node_count; i++) {
					if (graph->nodes[i] == edge->src) {
						src = i;
					}
					if (graph->nodes[i] == edge->dest) {
						dest = i;
					}
				}
				remove_edge(graph, edge);
				ASSERT(src >= 0 && dest >= 0);
				if (src >= 0 && dest >= 0) {
					adjacency[src][dest]--;
				}
			}

			check_graph(graph, adjacency, node_count);
		}

		free_graph(graph);
	}
}

// Random node removals on the graph. Removing a node must also remove its
// edges and keep every remaining pair's reachability intact.
TEST(test_fuzz_graph_remove_node_stays_consistent) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct Graph *graph = create_graph();
		ASSERT_NOT_NULL(graph);

		const int initial_nodes = 8;
		for (int i = 0; i < initial_nodes; i++) {
			add_node(graph, (uint64_t)i);
		}

		int adjacency[8][8] = {0};
		int node_count = initial_nodes;

		// Wire up a random graph first.
		for (int e = 0; e < 30; e++) {
			int src = (int)rng_below((uint32_t)node_count);
			int dest = (int)rng_below((uint32_t)node_count);
			add_edge(graph, graph->nodes[src], graph->nodes[dest],
					 (int)rng_below(10));
			adjacency[src][dest]++;
		}

		check_graph(graph, adjacency, node_count);

		// Then remove nodes one by one.
		while (node_count > 1) {
			int victim = (int)rng_below((uint32_t)node_count);
			remove_node(graph, graph->nodes[victim]);

			// Delete the row, the column and every edge of the shadow
			// matrix, mirroring what the graph did.
			for (int j = 0; j < node_count; j++) {
				adjacency[victim][j] = 0;
			}
			for (int i = 0; i < node_count; i++) {
				adjacency[i][victim] = 0;
			}
			for (int i = victim; i < node_count - 1; i++) {
				for (int j = 0; j < node_count; j++) {
					adjacency[i][j] = adjacency[i + 1][j];
				}
			}
			for (int j = victim; j < node_count - 1; j++) {
				for (int i = 0; i < node_count - 1; i++) {
					adjacency[i][j] = adjacency[i][j + 1];
				}
			}
			node_count--;

			check_graph(graph, adjacency, node_count);
		}

		free_graph(graph);
	}
}

// Random trees built with the insert helpers. After every step the size, the
// parent pointers and the height bounds must hold.
TEST(test_fuzz_btree_stays_consistent) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct BinaryTree *root = create_btree(rng_payload());
		ASSERT_NOT_NULL(root);

		struct BinaryTree *nodes[512] = {root};
		int node_count = 1;

		for (int op = 0; op < FUZZ_OPS; op++) {
			// Pick a random node and fill one of its empty slots.
			int pick = (int)rng_below((uint32_t)node_count);
			struct BinaryTree *target = nodes[pick];

			bool left = btree_get_left(target) == NULL;
			bool right = btree_get_right(target) == NULL;

			if (!left && !right) {
				continue;
			}

			bool use_left = left && (!right || rng_below(2) == 0);
			void *val = rng_payload();

			struct BinaryTree *child =
				use_left ? btree_insert_left(target, val)
						 : btree_insert_right(target, val);
			ASSERT_NOT_NULL(child);
			nodes[node_count++] = child;

			// Size, parents and height bounds after every step.
			ASSERT_EQ_INT(btree_size(root), node_count);
			ASSERT_EQ_INT(check_btree_node(root, NULL, 0), node_count);
			ASSERT(btree_height(root) >= 0);
			ASSERT(btree_height(root) <= node_count - 1);
		}

		btree_free(root);
	}
}

// Random n-ary trees. Every node must report its own number of children, and
// the total node count must match the number of inserts.
TEST(test_fuzz_nary_tree_stays_consistent) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct Tree *root = create_tree(rng_payload());
		ASSERT_NOT_NULL(root);

		// children[i] counts the children of nodes[i].
		struct Tree *nodes[512] = {root};
		int children[512] = {0};
		int node_count = 1;

		for (int op = 0; op < FUZZ_OPS; op++) {
			int pick = (int)rng_below((uint32_t)node_count);
			struct Tree *child = tree_add_child(nodes[pick], rng_payload());
			ASSERT_NOT_NULL(child);

			nodes[node_count] = child;
			children[node_count] = 0;
			children[pick]++;
			node_count++;

			ASSERT_EQ_INT(tree_num_children(nodes[pick]), children[pick]);
		}

		// Every node reports the right child count at the end as well.
		for (int i = 0; i < node_count; i++) {
			ASSERT_EQ_INT(tree_num_children(nodes[i]), children[i]);
		}

		tree_free(root);
	}
}

// Invalid indices and empty-container operations must leave the containers
// untouched rather than corrupting them.
TEST(test_fuzz_invalid_operations_are_harmless) {
	uint64_t seeds[8];
	int seed_count = 0;
	fuzz_setup(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		struct Array *a = create_array();
		struct SLList *sll = create_sll();
		struct DLList *dll = create_dll();

		// One round of clearly invalid calls per seed. The guards print a
		// diagnostic, so more rounds would flood the console on Windows.
		void *val = rng_payload();

		arr_insert(a, val, 100 + (int)rng_below(50));
		arr_remove(a, 100 + (int)rng_below(50));
		ASSERT_NULL(arr_get(a, 100 + (int)rng_below(50)));

		sll_insert(sll, val, 100 + (int)rng_below(50));
		sll_remove(sll, 100 + (int)rng_below(50));
		sll_remove(sll, -2 - (int)rng_below(10));

		dll_insert(dll, val, 100 + (int)rng_below(50));
		dll_remove(dll, 100 + (int)rng_below(50));
		dll_remove(dll, -2 - (int)rng_below(10));

		check_array(a, NULL, 0);
		check_sll(sll, NULL, 0);
		check_dll(dll, NULL, 0);

		free_array(a);
		free_sll(sll);
		free_dll(dll);
	}
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

void run_fuzz_tests(void) {
	test_section("Fuzz / random operation sequences");

	RUN_TEST(test_fuzz_array_matches_model);
	RUN_TEST(test_fuzz_sll_matches_model);
	RUN_TEST(test_fuzz_dll_matches_model);
	RUN_TEST(test_fuzz_stack_is_lifo);
	RUN_TEST(test_fuzz_queue_is_fifo);
	RUN_TEST(test_fuzz_hashmap_matches_model);
	RUN_TEST(test_fuzz_hashset_matches_model);
	RUN_TEST(test_fuzz_heap_pops_minimum);
	RUN_TEST(test_fuzz_graph_matches_model);
	RUN_TEST(test_fuzz_graph_remove_node_stays_consistent);
	RUN_TEST(test_fuzz_btree_stays_consistent);
	RUN_TEST(test_fuzz_nary_tree_stays_consistent);
	RUN_TEST(test_fuzz_invalid_operations_are_harmless);
}
