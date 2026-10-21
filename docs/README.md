# DraconisDSA

Data Structures & Algorithms library written in C23. Educational, dependency-free, and split into one small module per structure (each with its own header and translation unit).

## Structures

| Module            | Description                                                          |
| ----------------- | -------------------------------------------------------------------- |
| `src/array`       | Dynamic array (`arr_`-prefixed API), doubling growth                 |
| `src/sllist`      | Singly linked list (`sll_`-prefixed API)                             |
| `src/dllist`      | Doubly linked list (`dll_`-prefixed API)                             |
| `src/stack`       | LIFO stack built on `dllist`                                         |
| `src/queue`       | FIFO queue built on `dllist`                                         |
| `src/heap`        | Binary min-heap (`heap_`-prefixed API), comparator driven            |
| `src/hashmap`     | String-keyed hash map, DJB2 hash + separate chaining                 |
| `src/hashset`     | Hash set of unique strings, DJB2 hash + separate chaining            |
| `src/graph`       | Directed weighted graph (BFS `path_exists`, connectivity)            |
| `src/trees`       | Binary tree (`btree_`) + n-ary tree (`tree_`), pre/in/post-order     |

## Algorithms

| Module                 | Description                                                          |
| ---------------------- | -------------------------------------------------------------------- |
| `src/algorithms/sort`  | Bubble, selection, insertion, quick, heap, merge and counting sort    |
| `src/algorithms/search`| Linear search and binary search                                      |
| `src/algorithms/traversal` | BFS, DFS, BFS shortest path and Dijkstra's algorithm             |

`src/algorithms/algorithms.h` is an umbrella header that includes all three.

All of the generic algorithms are driven by a comparator with the same
signature as `qsort()`:

```c
typedef int (*cmp_fn)(const void *a, const void *b);
```

The comparator returns a negative number if `a` sorts before `b`, zero if they
are equal, and a positive number if `a` sorts after `b`. The sorts always
produce ascending order, so flipping the comparator flips the result.

## Building

Use `make` to compile the library and demo.

```sh
make
```

`src/main.c` is a small demo that exercises every structure and algorithm in
the library.

## Testing

The test suite lives in `tests/` and uses a small dependency-free harness. To
run it:

```sh
make test
```

This builds `tests.exe` and runs it. The report is printed at the end, listing
the number of tests and assertions that passed.

To run the same tests under AddressSanitizer and UndefinedBehaviorSanitizer:

```sh
make test-sanitize
```

That builds into `build-sanitize/` with `-fsanitize=address,undefined` and
fails the build if the tests report a leak, a memory error or undefined
behaviour. See [tests/README.md](../tests/README.md) for how to add a test.

```sh
make clean          # Remove all build artefacts
```

## Usage

Each module lives in its own folder and exposes its types in the matching header.
The sequence containers (array, singly/doubly linked lists, both tree types) store
`void *` payloads and leave ownership of those payloads to the caller (only the
container's nodes/buffer are freed). The hash structures and the graph differ:
`HashMap` stores `int` values, `HashSet` owns duplicated key strings, and `Graph`
stores `uint64_t` node data.

Lists accept insertion indices `0..size` and removal indices `0..size-1`
(`sll_insert`/`dll_insert` also accept `-1` as an "insert at back" shortcut).
Every allocator failure is handled and reported; empty-pop/peek and empty-dequeue
return `NULL`.

The binary heap stores `void *` payloads that the caller owns, and its order is
decided by the comparator it was created with.

`algo_dijkstra()` requires every edge weight to be zero or positive and returns
`false` otherwise. `algo_bfs_shortest_path()` writes into a buffer that the
caller provides, sized for one pointer per node in the graph.

## Documentation

- [Explanations](../explainations/README.en.md) - a walkthrough of every structure and algorithm
- [Complexity cheat sheet](COMPLEXITY.md) - the time complexity of every operation
- [Roadmap](ROADMAP.md) - what is planned next
- [Changelog](CHANGELOG.md) - what changed

## Memory

Everything created with `create_*` must be released with `free_*` (e.g. `free_sll`).

## License

GPL-3.0. See [LICENSE](../LICENSE).
