# Algorithms - Explained

This module holds the classic algorithms that are worth knowing by heart:
**sorting**, **searching** and **graph traversal**. They are grouped in three
headers, plus an umbrella header that pulls in all of them:

| Header                     | Contains                                            |
| -------------------------- | --------------------------------------------------- |
| `algorithms/sort.h`        | Six sorting algorithms                              |
| `algorithms/search.h`      | Linear search and binary search                     |
| `algorithms/traversal.h`   | BFS, DFS, shortest path and Dijkstra                |
| `algorithms/algorithms.h`  | Convenience header that includes all three          |

## The comparator

Every generic algorithm in this library is driven by a **comparator**, a
function pointer with the same signature that `qsort()` uses:

```c
typedef int (*cmp_fn)(const void *a, const void *b);
```

The comparator returns a negative number if `a` sorts before `b`, zero if they
are equal, and a positive number if `a` sorts after `b`. The algorithms here
always sort in *ascending* order as defined by the comparator, so swapping the
comparator flips the result.

The arrays being sorted are arrays of `void *`. The comparator is what makes it
possible to sort structs, strings, or anything else without giving up type
safety at the call site.

## Sorting

### The sorts

| Algorithm    | Idea                                                        | Best       | Average    | Worst      | Space | Stable |
| ------------ | ----------------------------------------------------------- | ---------- | ---------- | ---------- | ----- | ------ |
| Bubble sort  | Swap neighbours that are out of order, over and over         | O(n)       | O(n²)      | O(n²)      | O(1)  | Yes    |
| Selection sort| Find the smallest remaining element and swap it into place   | O(n²)      | O(n²)      | O(n²)      | O(1)  | No     |
| Insertion sort| Grow a sorted prefix, shifting each new element into place  | O(n)       | O(n²)      | O(n²)      | O(1)  | Yes    |
| Quick sort   | Partition around a pivot and sort both halves                | O(n log n) | O(n log n) | O(n²)      | O(log n) | No  |
| Heap sort    | Build a max-heap, then move the largest to the back          | O(n log n) | O(n log n) | O(n log n) | O(1)  | No     |
| Merge sort   | Split in half, sort both halves, merge                       | O(n log n) | O(n log n) | O(n log n) | O(n)  | Yes    |
| Counting sort| Count the occurrences, then place the values in order         | O(n + k)   | O(n + k)   | O(n + k)   | O(k)  | Yes    |

`k` is the size of the range between the smallest and the largest value.

### Implementation details

- **All sorts except counting sort** take `void **items`, a `size_t count` and
  a `cmp_fn`, sort the array **in place**, and return `true` on success or
  `false` if the arguments were invalid (`items == NULL` or `cmp == NULL`).
- **Bubble sort** walks the array and swaps neighbours that are out of order.
  It tracks whether a swap happened and stops early if a whole pass made no
  swaps - that is what gives it O(n) on already sorted input.
- **Selection sort** scans the unsorted part for the smallest element and
  swaps it to the front of that part.
- **Insertion sort** keeps a sorted prefix at the front and shifts each new
  element left until it is in the right place. It is the fastest of the
  quadratic sorts on nearly sorted data.
- **Quick sort** picks the element in the **middle** of the range as the
  pivot. That detail matters: it keeps already sorted input from degenerating
  into the O(n²) worst case. The recursion is done with explicit
  `long long` indices so that very large arrays cannot overflow.
- **Heap sort** builds a max-heap in the array itself, starting in the middle
  and working towards the front (so both children of a node are already heaps
  when the node is sifted down). Then it repeatedly swaps the root to the back
  and repairs the heap.
- **Merge sort** allocates a temporary buffer the same size as the input and
  merges the two sorted halves into it. Taking the element from the left half
  on a tie is what makes it **stable**. It returns `false` if the buffer cannot
  be allocated.
- **Counting sort** (`algo_counting_sort_int`) is the odd one out: it works on
  a plain `int *` array instead of `void **`, because it needs to look at the
  actual values. It counts the occurrences of every value in the range, turns
  the counts into ending positions, and then fills a temporary output array
  **backwards**. Going backwards is what keeps it stable, and it also avoids
  overwriting a value that has not been read yet. It refuses to run if the
  range is too large to fit in a bucket array.

### Searching

| Algorithm        | Requirement      | Time Complexity | Returns                     |
| ---------------- | ---------------- | --------------- | --------------------------- |
| `algo_linear_search()` | Any array         | O(n)            | Index of first match or -1  |
| `algo_binary_search()` | Sorted array      | O(log n)        | Index of a match or -1      |

- **Linear search** walks the array from front to back. It is the only option
  for an unsorted array, and it is faster than binary search for very small
  arrays.
- **Binary search** repeatedly halves the range of the search. It only works
  on an array that is already sorted **with the same comparator** that is
  passed in - sorting with one comparator and searching with another gives
  wrong answers.

### Graph traversal

| Algorithm                   | Finds                                            | Time Complexity |
| --------------------------- | ------------------------------------------------ | --------------- |
| `algo_bfs()`                | Every reachable node                             | O(V + E)        |
| `algo_dfs()`                | Every reachable node                             | O(V + E)        |
| `algo_bfs_shortest_path()`  | The path with the fewest edges                   | O(V + E)        |
| `algo_dijkstra()`           | The cheapest path, using edge weights            | O((V + E) log V)|

- **BFS** keeps a queue of the nodes it still has to look at and always takes
  the oldest one from the front. That is what makes it visit level by level.
- **DFS** keeps a stack and always takes the newest node from the top. The
  implementation uses its **own stack instead of recursion**, so it cannot run
  out of stack space on a deep graph. The neighbours are pushed in reverse
  order so that they come back off the stack in the order they were found.
- **BFS shortest path** is a BFS that also remembers how every node was
  reached, so the path to the goal can be rebuilt afterwards. The path is
  written into a buffer that the caller provides, and that buffer has to be
  able to hold **one pointer per node in the graph**.
- **Dijkstra** always continues with the node that is currently closest to the
  start and relaxes the edges that lead out of it. It uses the
  [binary heap](Heap.en.md) from `src/heap` as a priority queue, so the
  "closest node" is always the top of the heap. It **requires all edge weights
  to be zero or positive** and returns `false` if any edge is negative.
  Nodes that cannot be reached get the distance `INT_MAX`.

The traversals work on the `struct Graph` from `src/graph`. Each node knows
its own position in the graph's node array through its `index` field, which
lets the algorithms look up a node's slot in O(1) instead of searching the
array.

### Traversal callbacks

The traversals do not return lists of nodes. Instead they call a callback for
every node they reach:

```c
typedef void (*algo_visit_fn)(struct GraphNode *node, void *context);
```

The `context` pointer is handed straight through to the callback, which is
how you pass state into a traversal without using a global variable.

## Pros

- One place for the classic algorithms, all with the same comparator style
- Every algorithm is documented and covered by the test suite
- The comparator makes the sorts work with any payload type
- The traversals are iterative, so they are safe on very deep graphs
- `algo_dijkstra()` uses a real binary heap, so it scales to large graphs

## Cons

- `void **` arrays are less type-safe than typed arrays
- Quick sort can degenerate to O(n²) on adversarial input
- Merge sort needs an extra buffer the size of the input
- Counting sort only works on integers and only pays off for small ranges
- Dijkstra does not handle negative edge weights (use Bellman-Ford for that)
- There is no "sort this struct by field X" sugar - you write the comparator

## When to use

- **Learning**: the quadratic sorts are the clearest way to understand what
  "O(n log n)" actually buys you
- **Small inputs**: insertion sort beats everything on arrays of a handful of
  elements
- **Nearly sorted data**: insertion sort is the natural choice
- **Stable ordering matters**: merge sort and counting sort
- **Already sorted data**: bubble sort detects it in one pass
- **Finding an element**: binary search on a sorted array, linear search
  otherwise
- **Reaching every node**: BFS for level order, DFS for exhaustive search
- **Shortest path**: BFS for fewest edges, Dijkstra for cheapest path

## Example

```c
#include "algorithms/algorithms.h"

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

static void print_node(struct GraphNode *node, void *context) {
    (void)context;
    printf("%llu ", (unsigned long long)node->data);
}

int main() {
    // Sorting
    int values[] = {5, 3, 9, 1, 7};
    void *items[5];
    for (int i = 0; i < 5; i++) {
        items[i] = &values[i];
    }
    algo_quick_sort(items, 5, cmp_int);
    for (int i = 0; i < 5; i++) {
        printf("%d ", *(int *)items[i]);
    }
    printf("\n");  // 1 3 5 7 9

    // Searching - the array has to be sorted with the same comparator
    int target = 7;
    printf("%ld\n", algo_binary_search(items, 5, &target, cmp_int));  // 3

    // Graph traversal
    struct Graph *graph = create_graph();
    add_node(graph, 1);
    add_node(graph, 2);
    add_node(graph, 3);
    add_edge(graph, graph->nodes[0], graph->nodes[1], 1);
    add_edge(graph, graph->nodes[1], graph->nodes[2], 1);

    algo_bfs(graph, graph->nodes[0], print_node, NULL);
    printf("\n");  // 1 2 3

    free_graph(graph);
    return 0;
}
```

## See also

- [Binary Heap](Heap.en.md) - the priority queue behind Dijkstra and heap sort
- [Graph](Graph.en.md) - the graph structure the traversals work on
- [Complexity cheat sheet](../docs/COMPLEXITY.md)
