/*
 * Benchmarks - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

/*
 * The Linux side needs clock_gettime, which strict -std=c2x hides, so ask
 * for POSIX 1993 (the timers option) explicitly. Harmless elsewhere.
 */
#define _POSIX_C_SOURCE 199309L

#include "algorithms/search.h"
#include "algorithms/sort.h"
#include "array/array.h"
#include "dllist/dllist.h"
#include "graph/graph.h"
#include "hashmap/hashmap.h"
#include "hashset/hashset.h"
#include "heap/heap.h"
#include "queue/queue.h"
#include "shared/shared.h"
#include "sllist/sllist.h"
#include "stack/stack.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

/*
 * Every benchmark runs a batch of exactly `n` operations and reports the
 * time per operation. The batch is then repeated with `2n` operations:
 * the "growth" column shows how the per-operation cost reacts to doubling
 * the workload, which is what separates the complexity classes:
 *
 *   growth ~ 1.0  -> O(1) amortized
 *   growth ~ 1.0-1.2 -> O(log n)
 *   growth ~ 2.0  -> O(n) per operation
 *
 * Each measurement repeats the batch until at least MEASURE_MS of work has
 * been done and keeps the fastest run, which keeps noisy first runs (cold
 * caches, lazy allocator warmup) out of the numbers.
 */

/** Minimum wall time spent per measurement before accepting a result. */
#define MEASURE_MS 60

/** A benchmark: runs a batch of `n` operations, returns the elapsed ns. */
typedef uint64_t (*bench_fn)(size_t n);

/** The signature every sorting algorithm in this library shares. */
typedef bool (*sort_fn)(void **, size_t, cmp_fn);

/** Collects search results so the optimizer cannot drop them. */
static volatile long bench_sink = 0;

// ---------------------------------------------------------------------------
// Timing
// ---------------------------------------------------------------------------

// Returns a monotonically increasing time in nanoseconds.
static uint64_t now_ns(void) {
#ifdef _WIN32
	// QueryPerformanceCounter: precise on every Windows since XP.
	LARGE_INTEGER freq;
	LARGE_INTEGER counter;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&counter);

	uint64_t secs = (uint64_t)counter.QuadPart / (uint64_t)freq.QuadPart;
	uint64_t rest = (uint64_t)counter.QuadPart % (uint64_t)freq.QuadPart;
	return secs * 1000000000ull + rest * 1000000000ull / (uint64_t)freq.QuadPart;
#else
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
#endif
}

// Deterministic generator for benchmark payloads (splitmix64).
static uint64_t bench_rng = 0x51D5EED5A11;

// Returns the next random 64-bit number.
static uint64_t rng_next(void) {
	uint64_t z = (bench_rng += 0x9E3779B97F4A7C15ULL);
	z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
	z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
	return z ^ (z >> 31);
}

// A block of int payloads to store pointers to.
#define PAYLOAD_POOL 262144
static int pool[PAYLOAD_POOL];

// Orders plain ints ascending.
static int cmp_int(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x > y) - (x < y);
}

// ---------------------------------------------------------------------------
// Container benchmarks: one batch = n operations
// ---------------------------------------------------------------------------

// Push n values onto the array (amortized O(1)).
static uint64_t bench_array_push(size_t n) {
	struct Array *a = create_array();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		arr_push(a, &pool[i % PAYLOAD_POOL]);
	}
	uint64_t elapsed = now_ns() - start;

	free_array(a);
	return elapsed;
}

// Pop n values off the array (O(1)).
static uint64_t bench_array_pop(size_t n) {
	struct Array *a = create_array();
	for (size_t i = 0; i < n; i++) {
		arr_push(a, &pool[i % PAYLOAD_POOL]);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		bench_sink += (arr_pop(a) != NULL);
	}
	uint64_t elapsed = now_ns() - start;

	free_array(a);
	return elapsed;
}

// Insert n values into the middle of the array (O(n) per insert).
static uint64_t bench_array_insert_middle(size_t n) {
	struct Array *a = create_array();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		arr_insert(a, &pool[i % PAYLOAD_POOL], (int)(a->size / 2));
	}
	uint64_t elapsed = now_ns() - start;

	free_array(a);
	return elapsed;
}

// Remove n values from the middle of the array (O(n) per remove).
static uint64_t bench_array_remove_middle(size_t n) {
	struct Array *a = create_array();
	for (size_t i = 0; i < n; i++) {
		arr_push(a, &pool[i % PAYLOAD_POOL]);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		arr_remove(a, (int)(a->size / 2));
	}
	uint64_t elapsed = now_ns() - start;

	free_array(a);
	return elapsed;
}

// Append n values to the singly linked list (O(1), tail pointer).
static uint64_t bench_sll_push_back(size_t n) {
	struct SLList *l = create_sll();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		sll_insert_at_back(l, &pool[i % PAYLOAD_POOL]);
	}
	uint64_t elapsed = now_ns() - start;

	free_sll(l);
	return elapsed;
}

// Prepend n values to the singly linked list (O(1)).
static uint64_t bench_sll_push_front(size_t n) {
	struct SLList *l = create_sll();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		sll_insert_at_front(l, &pool[i % PAYLOAD_POOL]);
	}
	uint64_t elapsed = now_ns() - start;

	free_sll(l);
	return elapsed;
}

// Remove the last node n times (O(n) walk from the head each time).
static uint64_t bench_sll_remove_back(size_t n) {
	struct SLList *l = create_sll();
	for (size_t i = 0; i < n; i++) {
		sll_insert_at_back(l, &pool[i % PAYLOAD_POOL]);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		sll_remove(l, (int)l->size - 1);
	}
	uint64_t elapsed = now_ns() - start;

	free_sll(l);
	return elapsed;
}

// Append n values to the doubly linked list (O(1)).
static uint64_t bench_dll_push_back(size_t n) {
	struct DLList *l = create_dll();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		dll_insert_at_back(l, &pool[i % PAYLOAD_POOL]);
	}
	uint64_t elapsed = now_ns() - start;

	free_dll(l);
	return elapsed;
}

// Prepend n values to the doubly linked list (O(1)).
static uint64_t bench_dll_push_front(size_t n) {
	struct DLList *l = create_dll();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		dll_insert_at_front(l, &pool[i % PAYLOAD_POOL]);
	}
	uint64_t elapsed = now_ns() - start;

	free_dll(l);
	return elapsed;
}

// Remove the last node n times (O(1), tail pointer).
static uint64_t bench_dll_remove_back(size_t n) {
	struct DLList *l = create_dll();
	for (size_t i = 0; i < n; i++) {
		dll_insert_at_back(l, &pool[i % PAYLOAD_POOL]);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		dll_remove(l, (int)l->size - 1);
	}
	uint64_t elapsed = now_ns() - start;

	free_dll(l);
	return elapsed;
}

// Remove the first node n times (O(1)).
static uint64_t bench_dll_remove_front(size_t n) {
	struct DLList *l = create_dll();
	for (size_t i = 0; i < n; i++) {
		dll_insert_at_back(l, &pool[i % PAYLOAD_POOL]);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		dll_remove(l, 0);
	}
	uint64_t elapsed = now_ns() - start;

	free_dll(l);
	return elapsed;
}

// n/2 stack pushes followed by n/2 stack pops (O(1) each).
static uint64_t bench_stack_push_pop(size_t n) {
	struct Stack *s = create_stack();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n / 2; i++) {
		stack_push(s, &pool[i % PAYLOAD_POOL]);
	}
	for (size_t i = 0; i < n / 2; i++) {
		bench_sink += (stack_pop(s) != NULL);
	}
	uint64_t elapsed = now_ns() - start;

	free_stack(s);
	return elapsed;
}

// n/2 enqueues followed by n/2 dequeues (O(1) each).
static uint64_t bench_queue_add_get(size_t n) {
	struct Queue *q = create_queue();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n / 2; i++) {
		queue_add(q, &pool[i % PAYLOAD_POOL]);
	}
	for (size_t i = 0; i < n / 2; i++) {
		bench_sink += (queue_get(q) != NULL);
	}
	uint64_t elapsed = now_ns() - start;

	free_queue(q);
	return elapsed;
}

// Builds the string keys used by the hash container benchmarks.
static char *make_keys(size_t n, size_t width) {
	char *keys = (char *)malloc(n * width);
	if (keys == NULL) {
		fprintf(stderr, "Error: benchmark out of memory\n");
		exit(1);
	}
	for (size_t i = 0; i < n; i++) {
		snprintf(&keys[i * width], width, "k%07zu", i);
	}
	return keys;
}

// Insert n keys into the hash map. The map keeps a fixed number of
// buckets, so the chains grow linearly with n.
static uint64_t bench_hashmap_insert(size_t n) {
	char *keys = make_keys(n, 16);
	struct HashMap *map = create_hashmap();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		map_insert(map, &keys[i * 16], (int)i);
	}
	uint64_t elapsed = now_ns() - start;

	free_hashmap(map);
	free(keys);
	return elapsed;
}

// Look up n keys that are present (chain walk grows linearly with n).
static uint64_t bench_hashmap_get(size_t n) {
	char *keys = make_keys(n, 16);
	struct HashMap *map = create_hashmap();
	for (size_t i = 0; i < n; i++) {
		map_insert(map, &keys[i * 16], (int)i);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		int found = 0;
		bench_sink += map_get(map, &keys[i * 16], &found);
		bench_sink += found;
	}
	uint64_t elapsed = now_ns() - start;

	free_hashmap(map);
	free(keys);
	return elapsed;
}

// Remove n keys that are present.
static uint64_t bench_hashmap_remove(size_t n) {
	char *keys = make_keys(n, 16);
	struct HashMap *map = create_hashmap();
	for (size_t i = 0; i < n; i++) {
		map_insert(map, &keys[i * 16], (int)i);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		map_remove(map, &keys[i * 16]);
	}
	uint64_t elapsed = now_ns() - start;

	free_hashmap(map);
	free(keys);
	return elapsed;
}

// Add n values to the hash set. The set keeps a fixed number of buckets,
// so the chains grow linearly with n.
static uint64_t bench_hashset_add(size_t n) {
	char *keys = make_keys(n, 16);
	struct HashSet *set = create_hashset();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		bench_sink += set_add(set, &keys[i * 16]);
	}
	uint64_t elapsed = now_ns() - start;

	free_hashset(set);
	free(keys);
	return elapsed;
}

// Look up n values that are present.
static uint64_t bench_hashset_contains(size_t n) {
	char *keys = make_keys(n, 16);
	struct HashSet *set = create_hashset();
	for (size_t i = 0; i < n; i++) {
		set_add(set, &keys[i * 16]);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		bench_sink += set_contains(set, &keys[i * 16]);
	}
	uint64_t elapsed = now_ns() - start;

	free_hashset(set);
	free(keys);
	return elapsed;
}

// Remove n values that are present.
static uint64_t bench_hashset_remove(size_t n) {
	char *keys = make_keys(n, 16);
	struct HashSet *set = create_hashset();
	for (size_t i = 0; i < n; i++) {
		set_add(set, &keys[i * 16]);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		bench_sink += set_remove(set, &keys[i * 16]);
	}
	uint64_t elapsed = now_ns() - start;

	free_hashset(set);
	free(keys);
	return elapsed;
}

// Insert n elements into the heap (O(log n) sift-up per push).
static uint64_t bench_heap_push(size_t n) {
	struct Heap *heap = create_heap(cmp_int);

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		heap_push(heap, &pool[(i * 7919) % PAYLOAD_POOL]);
	}
	uint64_t elapsed = now_ns() - start;

	free_heap(heap);
	return elapsed;
}

// Pop n elements off the heap (O(log n) sift-down per pop).
static uint64_t bench_heap_pop(size_t n) {
	struct Heap *heap = create_heap(cmp_int);
	for (size_t i = 0; i < n; i++) {
		heap_push(heap, &pool[(i * 7919) % PAYLOAD_POOL]);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		bench_sink += (heap_pop(heap) != NULL);
	}
	uint64_t elapsed = now_ns() - start;

	free_heap(heap);
	return elapsed;
}

// Add n nodes to the graph (amortized O(1) node array growth).
static uint64_t bench_graph_add_node(size_t n) {
	struct Graph *graph = create_graph();

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		add_node(graph, i);
	}
	uint64_t elapsed = now_ns() - start;

	free_graph(graph);
	return elapsed;
}

// Add n edges to the graph (amortized O(1) edge array growth).
static uint64_t bench_graph_add_edge(size_t n) {
	struct Graph *graph = create_graph();
	add_node(graph, 1);
	add_node(graph, 2);

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		add_edge(graph, graph->nodes[0], graph->nodes[1], 1);
	}
	uint64_t elapsed = now_ns() - start;

	free_graph(graph);
	return elapsed;
}

// Ask has_edge n times on a graph with n edges; the edge list is scanned
// from the front every time, and no edge matches, so every query walks
// the whole list (O(n) per query).
static uint64_t bench_graph_has_edge(size_t n) {
	struct Graph *graph = create_graph();
	add_node(graph, 1);
	add_node(graph, 2);
	for (size_t i = 0; i < n; i++) {
		add_edge(graph, graph->nodes[0], graph->nodes[1], 1);
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		bench_sink += has_edge(graph, graph->nodes[1], graph->nodes[0]);
	}
	uint64_t elapsed = now_ns() - start;

	free_graph(graph);
	return elapsed;
}

// ---------------------------------------------------------------------------
// Algorithm benchmarks: one batch = one run over n elements
// ---------------------------------------------------------------------------

// Shuffles a fresh int payload array with the benchmark RNG.
static void fill_shuffled(int *values, size_t n) {
	for (size_t i = 0; i < n; i++) {
		values[i] = (int)(rng_next() % 1000000);
	}
}

// Sorts n shuffled elements; the "operation" is one element finding its
// place, so the per-element cost of an O(n log n) sort barely moves when
// n doubles while a quadratic sort doubles it.
static uint64_t sort_bench(size_t n, sort_fn which) {
	int *values = (int *)malloc(n * sizeof(int));
	void **items = (void **)malloc(n * sizeof(void *));
	if (values == NULL || items == NULL) {
		fprintf(stderr, "Error: benchmark out of memory\n");
		exit(1);
	}

	fill_shuffled(values, n);
	for (size_t i = 0; i < n; i++) {
		items[i] = &values[i];
	}

	uint64_t start = now_ns();
	which(items, n, cmp_int);
	uint64_t elapsed = now_ns() - start;

	free(items);
	free(values);
	return elapsed;
}

static uint64_t bench_bubble_sort(size_t n) { return sort_bench(n, algo_bubble_sort); }
static uint64_t bench_selection_sort(size_t n) { return sort_bench(n, algo_selection_sort); }
static uint64_t bench_insertion_sort(size_t n) { return sort_bench(n, algo_insertion_sort); }
static uint64_t bench_quick_sort(size_t n) { return sort_bench(n, algo_quick_sort); }
static uint64_t bench_heap_sort(size_t n) { return sort_bench(n, algo_heap_sort); }
static uint64_t bench_merge_sort(size_t n) { return sort_bench(n, algo_merge_sort); }

// Counting sort on n plain integers (O(n + k)).
static uint64_t bench_counting_sort(size_t n) {
	int *values = (int *)malloc(n * sizeof(int));
	if (values == NULL) {
		fprintf(stderr, "Error: benchmark out of memory\n");
		exit(1);
	}

	for (size_t i = 0; i < n; i++) {
		values[i] = (int)(rng_next() % 256);
	}

	uint64_t start = now_ns();
	algo_counting_sort_int(values, n);
	uint64_t elapsed = now_ns() - start;

	free(values);
	return elapsed;
}

// Linear search for a value that is not in the array: every query walks
// the whole array (O(n) per query).
static uint64_t bench_linear_search(size_t n) {
	int *values = (int *)malloc(n * sizeof(int));
	void **items = (void **)malloc(n * sizeof(void *));
	if (values == NULL || items == NULL) {
		fprintf(stderr, "Error: benchmark out of memory\n");
		exit(1);
	}

	int missing = 1000000;
	for (size_t i = 0; i < n; i++) {
		values[i] = (int)i;
		items[i] = &values[i];
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		bench_sink += algo_linear_search(items, n, &missing, cmp_int);
	}
	uint64_t elapsed = now_ns() - start;

	free(items);
	free(values);
	return elapsed;
}

// Binary search for a value that is not in a sorted array (O(log n) per
// query).
static uint64_t bench_binary_search(size_t n) {
	int *values = (int *)malloc(n * sizeof(int));
	void **items = (void **)malloc(n * sizeof(void *));
	if (values == NULL || items == NULL) {
		fprintf(stderr, "Error: benchmark out of memory\n");
		exit(1);
	}

	// Even numbers, so the odd target is always missing.
	int missing = -1;
	for (size_t i = 0; i < n; i++) {
		values[i] = (int)(2 * i);
		items[i] = &values[i];
	}

	uint64_t start = now_ns();
	for (size_t i = 0; i < n; i++) {
		bench_sink += algo_binary_search(items, n, &missing, cmp_int);
	}
	uint64_t elapsed = now_ns() - start;

	free(items);
	free(values);
	return elapsed;
}

// ---------------------------------------------------------------------------
// The driver
// ---------------------------------------------------------------------------

/** One row of the report. */
struct BenchEntry {
	/** Label printed in the table. */
	const char *name;
	/** The benchmark. */
	bench_fn fn;
	/** Batch size of the first measurement; the second doubles it. */
	size_t base_n;
	/** What one "operation" is: a single call or one element. */
	const char *unit;
};

static const struct BenchEntry entries[] = {
	{"array push", bench_array_push, 50000, "op"},
	{"array pop", bench_array_pop, 50000, "op"},
	{"array insert (middle)", bench_array_insert_middle, 2000, "op"},
	{"array remove (middle)", bench_array_remove_middle, 2000, "op"},
	{"sll insert at back", bench_sll_push_back, 50000, "op"},
	{"sll insert at front", bench_sll_push_front, 50000, "op"},
	{"sll remove last", bench_sll_remove_back, 2000, "op"},
	{"dll insert at back", bench_dll_push_back, 50000, "op"},
	{"dll insert at front", bench_dll_push_front, 50000, "op"},
	{"dll remove last", bench_dll_remove_back, 50000, "op"},
	{"dll remove first", bench_dll_remove_front, 50000, "op"},
	{"stack push+pop", bench_stack_push_pop, 50000, "op"},
	{"queue add+get", bench_queue_add_get, 50000, "op"},
	{"hashmap insert", bench_hashmap_insert, 5000, "op"},
	{"hashmap get (hit)", bench_hashmap_get, 5000, "op"},
	{"hashmap remove", bench_hashmap_remove, 5000, "op"},
	{"hashset add", bench_hashset_add, 5000, "op"},
	{"hashset contains (hit)", bench_hashset_contains, 5000, "op"},
	{"hashset remove", bench_hashset_remove, 5000, "op"},
	{"heap push", bench_heap_push, 50000, "op"},
	{"heap pop", bench_heap_pop, 50000, "op"},
	{"graph add node", bench_graph_add_node, 50000, "op"},
	{"graph add edge", bench_graph_add_edge, 50000, "op"},
	{"graph has edge (miss)", bench_graph_has_edge, 2000, "op"},
	{"bubble sort", bench_bubble_sort, 1000, "elem"},
	{"selection sort", bench_selection_sort, 1000, "elem"},
	{"insertion sort", bench_insertion_sort, 1000, "elem"},
	{"quick sort", bench_quick_sort, 20000, "elem"},
	{"heap sort", bench_heap_sort, 20000, "elem"},
	{"merge sort", bench_merge_sort, 20000, "elem"},
	{"counting sort (ints)", bench_counting_sort, 200000, "elem"},
	{"linear search (miss)", bench_linear_search, 1000, "op"},
	{"binary search (miss)", bench_binary_search, 100000, "op"},
};

/** Number of rows in the report. */
static const int entry_count = (int)(sizeof(entries) / sizeof(entries[0]));

// Repeats a benchmark until MEASURE_MS of work is done and returns the
// fastest batch time in nanoseconds.
static uint64_t best_batch_ns(bench_fn fn, size_t n) {
	uint64_t budget = MEASURE_MS * 1000000ull;
	uint64_t best = 0;
	uint64_t spent = 0;

	while (spent < budget) {
		uint64_t t = fn(n);
		if (t < 1) {
			t = 1;
		}
		if (best == 0 || t < best) {
			best = t;
		}
		spent += t;
	}

	return best;
}

// Times one entry at n and 2n and prints the row.
static void run_entry(const struct BenchEntry *entry) {
	uint64_t small = best_batch_ns(entry->fn, entry->base_n);
	uint64_t large = best_batch_ns(entry->fn, entry->base_n * 2);

	double per_op_small = (double)small / (double)entry->base_n;
	double per_op_large = (double)large / (double)(entry->base_n * 2);
	double growth = per_op_large / per_op_small;

	printf("%-26s %8zu %-4s %11.1f %11.1f %8.2fx\n", entry->name,
		   entry->base_n, entry->unit, per_op_small, per_op_large, growth);
}

// The benchmark program: fills the payload pool once, prints the table
// header, runs every entry and prints the total time.
int main(void) {
	for (int i = 0; i < PAYLOAD_POOL; i++) {
		pool[i] = i;
	}

	uint64_t start = now_ns();

	printf("\nDraconisDSA benchmark suite\n");
	printf("==========================================================\n");
	printf("Each batch repeats until at least %d ms of work is done;\n",
		   MEASURE_MS);
	printf("the fastest batch is used. 'growth' is the per-operation cost\n");
	printf("of doubling the workload: 1.0 = constant, ~2.0 = linear.\n");
	printf("----------------------------------------------------------\n");
	printf("%-26s %8s %-4s %11s %11s %8s\n", "benchmark", "n", "unit",
		   "ns/op", "ns/op @2n", "growth");
	printf("----------------------------------------------------------\n");

	for (int i = 0; i < entry_count; i++) {
		run_entry(&entries[i]);
	}

	double total = (double)(now_ns() - start) / 1000000.0;

	printf("----------------------------------------------------------\n");
	printf("total time: %.1f ms\n", total);
	printf("(sink check: %ld)\n\n", (long)bench_sink);

	return 0;
}
