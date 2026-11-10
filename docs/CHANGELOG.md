# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Added

- Dynamic array (`src/array`) with a `void*` API: `create_array`, `arr_size`, `arr_is_empty`, `arr_push`, `arr_insert`, `arr_get`, `arr_set`, `arr_remove`, `arr_pop`, `print_array`, `free_array`. Grows by doubling its capacity for amortized O(1) appends.
- Array demo in `src/main.c`.
- Dynamic array explanation (`explainations/Array.en.md`).
- Singly linked list (`src/sllist`) with a `void*` API: `create_sll`, `sll_is_empty`, `sll_get_idx`, `sll_size`, `sll_remove`, `sll_insert`, `sll_insert_at_front`, `sll_insert_at_back`, `print_sll`, `free_sll`.
- Doubly linked list (`src/dllist`) with a `void*` API: `create_dll`, `dll_is_empty`, `dll_get_idx`, `dll_size`, `dll_remove`, `dll_insert`, `dll_insert_at_front`, `dll_insert_at_back`, `print_dll`, `free_dll`. Keeps a tail pointer, so appending is O(1).
- Stack (`src/stack`) built on the doubly linked list: `create_stack`, `stack_push`, `stack_pop`, `stack_peek`, `free_stack`.
- Queue (`src/queue`) built on the doubly linked list: `create_queue`, `queue_add`, `queue_get`, `queue_peek`, `free_queue`.
- Hash map (`src/hashmap`) with a string-keyed API: `create_hashmap`, `map_insert`, `map_get`, `map_remove`, `hashmap_hash`, `free_hashmap`. Uses DJB2 hashing with separate chaining.
- Hash set (`src/hashset`) for unique strings: `create_hashset`, `set_add`, `set_contains`, `set_remove`, `set_hash`, `free_hashset`. Uses DJB2 hashing with separate chaining.
- Graph (`src/graph`): `create_graph`, `add_node`, `add_edge`, `remove_node`, `remove_edge`, `has_node`, `has_edge`, `path_exists`, `connected`, `print_graph`, `free_graph`. Nodes store `uint64_t` data, edges store a `src`, a `dest` and an `int` weight.
- Trees (`src/trees`): a binary tree (`create_btree`, `btree_insert_left`, `btree_insert_right`, `btree_get_*`, `btree_set_*`, `btree_is_leaf`, `btree_height`, `btree_size`, `btree_traverse_preorder/inorder/postorder`, `btree_free`) and an n-ary tree (`create_tree`, `tree_add_child`, `tree_num_children`, `tree_free`).
- **Binary heap** (`src/heap`): `create_heap`, `heap_push`, `heap_pop`, `heap_peek`, `heap_size`, `heap_is_empty`, `free_heap`. The order is decided by a comparator, so the same code is a min-heap or a max-heap depending on the comparator that is passed in.
- **Algorithms module** (`src/algorithms`):
    - Sorting (`sort.h`): `algo_bubble_sort`, `algo_selection_sort`, `algo_insertion_sort`, `algo_quick_sort`, `algo_heap_sort`, `algo_merge_sort` and `algo_counting_sort_int`.
    - Searching (`search.h`): `algo_linear_search`, `algo_binary_search`.
    - Graph traversal (`traversal.h`): `algo_bfs`, `algo_dfs`, `algo_bfs_shortest_path`, `algo_dijkstra`.
    - `algorithms.h` as an umbrella header for all three.
- **Shared comparator type** `cmp_fn` in `src/shared/shared.h`, matching the signature of `qsort()`.
- **Test suite** (`tests/`): a dependency-free harness with `TEST`/`RUN_TEST` macros and the assertions `ASSERT`, `ASSERT_FALSE`, `ASSERT_EQ_INT`, `ASSERT_EQ_LONG`, `ASSERT_EQ_UINT`, `ASSERT_EQ_STR`, `ASSERT_NULL`, `ASSERT_NOT_NULL` and `ASSERT_PTR_EQ`. Covers every module, including the heap and the algorithms.
- **Makefile** targets: `all`, `test`, `test-sanitize` and `clean`. The library and the demo are linked separately, so the test binary can link the library without a duplicate `main()`. `test-sanitize` builds into `build-sanitize/` with AddressSanitizer and UndefinedBehaviorSanitizer.
- **Demo** (`src/main.c`) rewritten as a sectioned showcase that exercises every structure and algorithm.
- **Documentation**:
    - `explainations/Heap.en.md` - the binary heap explained.
    - `explainations/Algorithms.en.md` - sorting, searching and graph traversal explained.
    - `docs/COMPLEXITY.md` - a complexity cheat sheet covering every public operation.
    - `docs/ROADMAP.md` - what is planned next.
    - `tests/README.md` - how to write and run a test.
    - Continuous integration in `.github/workflows/ci.yml`, running the build, the tests and the sanitized tests on every push and pull request.
- **Fuzz and property tests** (`tests/test_fuzz.c`, `tests/test_sort_property.c`): randomized inputs against every container with invariant checks after each operation, and the six sorts plus counting sort checked against a `qsort()` oracle, including a stability check. A failing random seed can be replayed with the `DRACONIS_FUZZ_SEED` environment variable.
- **Benchmark suite** (`benchmarks/`): `make bench` measures every operation with best-of-repeats timing and a growth check at n and 2n, so the complexity table can be checked against reality. CI runs it on every push.
- **Valgrind in CI**: a dedicated `valgrind` job runs `make test-valgrind` (leak check with `--error-exitcode`) on Ubuntu.
- **API reference**: `make docs` generates the Doxygen HTML reference into `docs/api/`, and `.github/workflows/docs.yml` deploys it to GitHub Pages.
- **ASCII diagrams** in the explanations: the shape of the singly and doubly linked list, the trees, the graph's flat node/edge arrays, and the heap's array-as-tree index mapping.
- **Worked examples** in `explainations/Algorithms.en.md`: insertion, quick, heap, merge and counting sort traced step by step, plus BFS, DFS and Dijkstra on concrete graphs. Every trace was produced by the real implementation.

### Fixed

- `remove_node()` in `src/graph/graph.c` removed the incident edges **before**
  freeing the node. It used to compare against the node after it had been
  freed, and it left the `index` field of the shifted nodes pointing at the
  wrong slot.
- `algo_counting_sort_int()` wrote the sorted values straight into the input
  array while walking it forwards, which could overwrite a value that had not
  been read yet. It now uses a temporary output array and walks backwards,
  which also makes it stable.
- `docs/COMPLEXITY.md` listed `sll_insert_at_back()` as O(n). The list keeps a
  tail pointer, so inserting at the back is O(1).
- The traversal complexities in `docs/COMPLEXITY.md` and
  `explainations/Algorithms.en.md` claimed the textbook O(V + E). Because the
  graph stores its edges in one flat array, BFS, DFS and BFS shortest path
  are O(V * E), and Dijkstra is O(V * E + E log E).

### Changed

- `struct GraphNode` no longer has an unused `next` field. It now has an
  `int index` that holds the node's position in the graph's node array, which
  lets the traversal algorithms look up a node's slot in O(1).
- The explanations for every structure now link to the complexity cheat sheet.
