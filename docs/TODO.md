# TODO

## Documentation - Full Tutorials
- [x] Write full tutorial with examples for: Singly Linked List (explainations/SLL.en.md)
- [x] Write full tutorial with examples for: Doubly Linked List (expand existing explainations/DLL.en.md)
- [x] Write full tutorial with examples for: Stack (explainations/Stack.en.md)
- [x] Write full tutorial with examples for: Queue (explainations/Queue.en.md)
- [x] Write full tutorial with examples for: HashMap (explainations/HashMap.en.md)
- [x] Write full tutorial with examples for: HashSet (explainations/HashSet.en.md)
- [x] Write full tutorial with examples for: Graph (explainations/Graph.en.md)
- [x] Write full tutorial with examples for: Trees (Binary + N-ary) (explainations/Trees.en.md)
- [x] Write full tutorial with examples for: Binary Heap (explainations/Heap.en.md)
- [x] Write full tutorial with examples for: Algorithms (explainations/Algorithms.en.md)
- [ ] Translate the explanations to Norwegian
- [ ] Add ASCII diagrams to the list, tree and graph explanations

## Code Documentation
- [x] Add Doxygen-style or comprehensive inline comments to all .h and .c files
- [x] Ensure consistent commenting style across codebase
- [ ] Generate an API reference from the Doxygen comments

## Docs Updates
- [x] Fill in ROADMAP.md
- [x] Update README.md with better examples
- [x] Fill in CHANGELOG.md
- [x] Add a complexity cheat sheet (docs/COMPLEXITY.md)

## Testing
- [x] Add a dependency-free test harness (tests/test.h)
- [x] Write tests for every module
- [x] Add `make test` and `make test-sanitize` targets
- [x] Add continuous integration (.github/workflows/ci.yml)
- [ ] Add fuzz tests that check the invariants after random operations
- [ ] Add a benchmark suite to measure the real cost of each operation

## Code
- [x] Add a binary heap (src/heap) to power heap sort and Dijkstra
- [x] Add an algorithms module (src/algorithms) with sorting, searching and traversal
- [x] Fix `remove_node()` in src/graph/graph.c comparing against a freed node
- [x] Fix `algo_counting_sort_int()` overwriting values it had not read yet
- [ ] Add a balanced binary search tree (src/bst)
- [ ] Add a deque (src/deque)
- [ ] Add a trie (src/trie)
- [ ] Add a disjoint set / union-find (src/dsu)
- [ ] Add Bellman-Ford and A* to src/algorithms/traversal
- [ ] Add topological sort and minimum spanning trees to src/algorithms/traversal
