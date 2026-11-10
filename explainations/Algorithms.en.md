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

### The sorts, step by step

Every trace below was produced by instrumented copies of the real code and
cross-checked against the library's own results for the same input.

**Insertion sort** on `[5, 2, 4, 1, 3]`. The sorted prefix at the front grows
by one element per round; each new element is lifted out and shifted left
until it sits in the right place:

```
start:   [5, 2, 4, 1, 3]
place 2: [2, 5]
place 4: [2, 4, 5]
place 1: [1, 2, 4, 5]
place 3: [1, 2, 3, 4, 5]
```

**Quick sort** on `[3, 8, 2, 5, 1, 4, 7, 6]`. This is Hoare partitioning with
the middle element as pivot: a left pointer stops at the first element that
does not belong before the pivot, a right pointer stops at the first that does
not belong after it, the two are swapped, and when the pointers meet, the
meeting point splits the range for the recursion:

```
sort [0..7]: [3, 8, 2, 5, 1, 4, 7, 6]
  pivot = items[3] = 5
  swap items[1] <-> items[5]: [3, 4, 2, 5, 1, 8, 7, 6]
  swap items[3] <-> items[4]: [3, 4, 2, 1, 5, 8, 7, 6]
  pointers meet -> split at 3
sort [0..3]: [3, 4, 2, 1, ...]
  pivot = items[1] = 4
  swap items[1] <-> items[3]: [3, 1, 2, 4, 5, 8, 7, 6]
  pointers meet -> split at 2
sort [0..2]: [3, 1, 2, ...]
  pivot = items[1] = 1
  swap items[0] <-> items[1]: [1, 3, 2, 4, 5, 8, 7, 6]
  pointers meet -> split at 0
sort [1..2]: [1, 3, 2, ...]
  pivot = items[1] = 3
  swap items[1] <-> items[2]: [1, 2, 3, 4, 5, 8, 7, 6]
  pointers meet -> split at 1
sort [4..7]: [..., 5, 8, 7, 6]
  pivot = items[5] = 8
  swap items[5] <-> items[7]: [1, 2, 3, 4, 5, 6, 7, 8]
  pointers meet -> split at 6
sort [4..6] and [4..5]: pivots 6 and 5, nothing left to move
sorted: [1, 2, 3, 4, 5, 6, 7, 8]
```

**Heap sort** on `[5, 3, 8, 1, 9, 2]`. First the array is turned into a
max-heap by sifting down from the middle, then the maximum is repeatedly
swapped to the back and the heap is repaired. The `|` marks where the sorted
tail begins:

```
start:          [5, 3, 8, 1, 9, 2]
build the heap:
  sift down 2:  [5, 3, 8, 1, 9, 2]   (8 has no larger child, stays)
  sift down 1:  [5, 9, 8, 1, 3, 2]   (3 swaps with 9)
  sift down 0:  [9, 5, 8, 1, 3, 2]   (5 swaps with 9)
extract the maximum:
  move 9 back:  [8, 5, 2, 1, 3 | 9]  (2 lands at the root, sifts past 8)
  move 8 back:  [5, 3, 2, 1 | 8, 9]
  move 5 back:  [3, 1, 2 | 5, 8, 9]
  move 3 back:  [2, 1 | 3, 5, 8, 9]  ([2, 1] is already in heap order)
  move 2 back:  [1 | 2, 3, 5, 8, 9]
sorted:         [1, 2, 3, 5, 8, 9]
```

**Merge sort** on `[5, 2, 4, 1, 3]`. The array is split down to single
elements, then the sorted runs are merged on the way back up. Taking the left
element on a tie is what makes each merge stable:

```
sort [5, 2, 4, 1, 3]
  sort [5, 2, 4]
    sort [5, 2]
      merge [5] with [2]        -> [2, 5]
    merge [2, 5] with [4]       -> [2, 4, 5]
  sort [1, 3]
    merge [1] with [3]          -> [1, 3]
  merge [2, 4, 5] with [1, 3]   -> [1, 2, 3, 4, 5]
```

**Counting sort** on `[3, 1, 4, 1, 5]`. The counts become ending positions,
and the array is then walked **backwards**, so equal values keep their
original order - watch the two 1s:

```
start:       [3, 1, 4, 1, 5]        values 1..5
counts:      1:2  2:0  3:1  4:1  5:1
positions:   1 ends at 2, 3 ends at 3, 4 ends at 4, 5 ends at 5
place 5 (from index 4) at output[4]
place 1 (from index 3) at output[1]
place 4 (from index 2) at output[3]
place 1 (from index 1) at output[0]
place 3 (from index 0) at output[2]
sorted:      [1, 1, 3, 4, 5]
```

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

| Algorithm                   | Finds                                            | Time Complexity  |
| --------------------------- | ------------------------------------------------ | ---------------- |
| `algo_bfs()`                | Every reachable node                             | O(V * E)         |
| `algo_dfs()`                | Every reachable node                             | O(V * E)         |
| `algo_bfs_shortest_path()`  | The path with the fewest edges                   | O(V * E)         |
| `algo_dijkstra()`           | The cheapest path, using edge weights            | O(V * E + E log E)|

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

The times above are what this implementation really costs: because the graph
keeps all of its edges in one flat array, every node a traversal processes
rescans the whole edge list to find its outgoing edges, which turns the
textbook O(V + E) into O(V * E). `algo_dijkstra()` pays the same edge scans
plus O(E log E) of work on its binary heap.

### Traversal callbacks

The traversals do not return lists of nodes. Instead they call a callback for
every node they reach:

```c
typedef void (*algo_visit_fn)(struct GraphNode *node, void *context);
```

The `context` pointer is handed straight through to the callback, which is
how you pass state into a traversal without using a global variable.

### The traversals, step by step

Both examples below were produced by the real implementations.

Take this graph (nodes in insertion order A to E, edges in insertion order
as listed):

```
      A ------> B ------> D
      |         |
      |         |
      v         v
      C ------> E

  edges: A->B  A->C  B->D  B->E  C->E
```

**BFS from A** uses a queue, so the visit order is level by level - A, then
everything one edge away, then everything two edges away:

```
visit A    queue after: B, C
visit B    queue after: C, D, E   (D and E discovered from B)
visit C    queue after: D, E      (E already seen, not queued twice)
visit D    queue after: E
visit E    queue after: (empty)

order: A B C D E
```

**DFS from A** uses a stack, so it dives down one branch before backing up.
The neighbours are pushed in reverse order, so the first edge found is the
first one followed:

```
visit A    stack after: B, C      (C pushed first, B on top)
visit B    stack after: C, E, D   (E pushed before D)
visit D    stack after: C, E      (D has no outgoing edges)
visit E    stack after: C
visit C    stack after: (empty)

order: A B D E C
```

**Shortest path A -> E** (`algo_bfs_shortest_path`) is the same BFS, but it
also remembers how every node was first reached: B and C from A, D and E
from B. Walking those parents backwards from E and reversing gives
`A B E` - two edges, and there is no shorter route: the only other way to
reach E is through C, which is also two edges away from A.

**Dijkstra** on a weighted graph. The edges are A->B (4), A->C (1), C->B (2),
B->D (1), C->D (5), D->E (1) and C->E (15). Every step takes the closest
unprocessed node from the heap and relaxes its outgoing edges:

```
step  take    relax outgoing edges
 1    A (0)   B: 0 + 4 = 4      C: 0 + 1 = 1
 2    C (1)   B: 1 + 2 = 3 < 4  D: 1 + 5 = 6   E: 1 + 15 = 16
 3    B (3)   D: 3 + 1 = 4 < 6
 4    D (4)   E: 4 + 1 = 5 < 16
 5    E (5)   (no outgoing edges)
```

(The heap still holds the outdated entries B = 4, D = 6 and E = 16 by the
end - those are skipped, because their recorded distance no longer matches
the best known distance.)

The final distances are `A: 0, B: 3, C: 1, D: 4, E: 5`. Drawn as routes, the
cheapest path to every node runs through C first - the direct A->B (4) loses
to the detour A->C->B (3):

```
      A --1--> C --2--> B
                        |
                      1 |
                        v
                        D
                        |
                      1 |
                        v
                        E
```

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
