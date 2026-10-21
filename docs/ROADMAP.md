# Roadmap

Where this library is headed. The guiding rule is that **every structure stays
small enough to read in one sitting** and that everything in it is covered by
tests.

## Done

- [x] Core containers: dynamic array, singly and doubly linked list
- [x] Stack and queue on top of the doubly linked list
- [x] Hash map and hash set (DJB2 + separate chaining)
- [x] Graph with weighted, directed edges
- [x] Binary tree and n-ary tree with the three traversal orders
- [x] Binary heap with a comparator-driven order
- [x] Algorithms: six sorts, two searches, BFS, DFS, shortest path, Dijkstra
- [x] Dependency-free test suite with a sanitized build
- [x] Explanations for every structure, plus a complexity cheat sheet
- [x] Continuous integration

## Next

### Algorithms

- [ ] **Balanced binary search tree** (`src/bst`) - keeps the tree balanced, so
      lookups are O(log n) instead of O(n) on an unbalanced tree
- [ ] **Hashing with open addressing** - an alternative to separate chaining,
      to show the other way of building a hash table
- [ ] **Bellman-Ford** - shortest paths with negative edge weights, as the
      natural follow-up to Dijkstra
- [ ] **A\*** - Dijkstra with a heuristic, for path finding on a grid
- [ ] **Topological sort** - ordering the nodes of a directed acyclic graph
- [ ] **Minimum spanning tree** (Kruskal and Prim)
- [ ] **B-tree / B+tree** - the structure that databases actually use

### Structures

- [ ] **Deque** - a double-ended queue, so both ends are O(1)
- [ ] **Priority queue** as a named API on top of the heap, with a
      "change priority" operation
- [ ] **Trie** - prefix tree for string lookup
- [ ] **Disjoint set (union-find)** - with path compression and union by rank
- [ ] **Skip list** - a probabilistic alternative to a balanced tree

### Quality

- [ ] **Fuzz tests** that feed random inputs to every container and check the
      invariants after each operation
- [ ] **Benchmark suite** that measures the real cost of each operation, so the
      complexity table can be checked against reality
- [ ] **Memory-leak checks in CI** with Valgrind on top of the sanitizers
- [ ] **Property-based testing** for the sorts, checking the result against
      `qsort()` from the standard library

### Documentation

- [ ] **Diagrams** for the list, tree and graph structures, drawn as ASCII art
      so they stay readable in a terminal
- [ ] **Worked examples** for each algorithm, showing the array or the graph
      after every step
- [ ] **Translations** of the explanations, starting with Norwegian
- [ ] **API reference** generated from the Doxygen comments

## Deliberately not planned

- **Templates or generics.** The library is plain C23 with `void *` payloads.
  That is the point: it shows what the machinery looks like without a type
  system hiding it.
- **External dependencies.** The test harness, the build system and the
  algorithms are all written from scratch so that the library can be dropped
  into any project as-is.
- **Thread safety.** None of the containers are thread safe. Adding locks
  would make the code much harder to read and the library is meant for
  learning.
- **Stable sort for the heap.** A heap cannot be stable without extra
  bookkeeping, and merge sort and counting sort already cover that case.

## Principles

1. **One folder per module.** Each structure gets its own header and
   translation unit, so a reader can pick up exactly one file at a time.
2. **No hidden dependencies.** A module only includes what it actually needs.
3. **Every operation documented.** Doxygen comments on every public function,
   including what it does when the arguments are invalid.
4. **Every module tested.** No module is merged without tests, and the
   sanitized build has to stay clean.
5. **Readable over fast.** Where a choice exists, the one that teaches
   something wins, as long as the complexity is still reasonable.
