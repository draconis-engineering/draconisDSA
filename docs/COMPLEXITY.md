# Complexity Cheat Sheet

One page with the time complexity of every public operation in the library.

**Legend**

- **O(1)** - constant time, does not depend on the size of the container
- **O(log n)** - logarithmic, doubles in size every step
- **O(n)** - linear, one pass over the data
- **O(n log n)** - linearithmic, the usual cost of a good sort
- **O(n²)** - quadratic, usually too slow for large inputs
- **O(V + E)** - one pass over the vertices plus one over the edges
- **amortized** - the average cost per operation over a long run of operations

`n` is the number of elements in the container, `V` is the number of graph
nodes, `E` is the number of graph edges and `k` is the size of the range
between the smallest and the largest value being sorted.

---

## Dynamic array - `src/array`

| Operation             | What it does             | Complexity                          |
| --------------------- | ------------------------ | ----------------------------------- |
| `create_array()`      | Create empty array       | O(1)                                |
| `arr_push()`          | Append to the end        | O(1) amortized (grows by doubling)  |
| `arr_insert()`        | Insert at an index       | O(n) - everything behind shifts     |
| `arr_get()`           | Read an element          | O(1)                                |
| `arr_set()`           | Overwrite an element     | O(1)                                |
| `arr_remove()`        | Remove an element        | O(n) - everything behind shifts     |
| `arr_pop()`           | Remove the last element  | O(1)                                |
| `arr_size()`          | Number of elements       | O(1)                                |
| `arr_is_empty()`      | Check whether it is empty| O(1)                                |
| `print_array()`       | Print the elements       | O(n)                                |
| `free_array()`        | Free the buffer          | O(1)                                |

## Singly linked list - `src/sllist`

| Operation                | What it does             | Complexity                        |
| ------------------------ | ------------------------ | --------------------------------- |
| `create_sll()`           | Create empty list        | O(1)                              |
| `sll_insert_at_front()`  | Insert at the head       | O(1)                              |
| `sll_insert_at_back()`   | Insert at the tail       | O(n) - walks to the end           |
| `sll_insert()`           | Insert at an index       | O(n) - walks to the index         |
| `sll_remove()`           | Remove at an index       | O(n) - walks to the index         |
| `sll_get_idx()`          | Find an element by value | O(n)                               |
| `sll_size()`             | Number of elements       | O(1) - the size is cached         |
| `sll_is_empty()`         | Check whether it is empty| O(1)                              |
| `print_sll()`            | Print the elements       | O(n)                              |
| `free_sll()`             | Free the nodes           | O(n)                              |

## Doubly linked list - `src/dllist`

| Operation                | What it does             | Complexity                        |
| ------------------------ | ------------------------ | --------------------------------- |
| `create_dll()`           | Create empty list        | O(1)                              |
| `dll_insert_at_front()`  | Insert at the head       | O(1)                              |
| `dll_insert_at_back()`   | Insert at the tail       | O(1) - the tail is tracked        |
| `dll_insert()`           | Insert at an index       | O(n) - walks to the index         |
| `dll_remove()`           | Remove at an index       | O(n) - walks to the index         |
| `dll_get_idx()`          | Find an element by value | O(n)                               |
| `dll_size()`             | Number of elements       | O(1) - the size is cached         |
| `dll_is_empty()`         | Check whether it is empty| O(1)                              |
| `print_dll()`            | Print the elements       | O(n)                              |
| `free_dll()`             | Free the nodes           | O(n)                              |

The double-linked list is the only list that can insert at the back in
constant time, because it keeps a pointer to the tail.

## Stack - `src/stack`

| Operation        | What it does             | Complexity |
| ---------------- | ------------------------ | ---------- |
| `create_stack()` | Create empty stack       | O(1)       |
| `stack_push()`   | Push onto the top        | O(1)       |
| `stack_pop()`    | Pop from the top         | O(1)       |
| `stack_peek()`   | Look at the top          | O(1)       |
| `free_stack()`   | Free the list            | O(n)       |

## Queue - `src/queue`

| Operation        | What it does             | Complexity |
| ---------------- | ------------------------ | ---------- |
| `create_queue()` | Create empty queue       | O(1)       |
| `queue_add()`    | Add to the rear          | O(1)       |
| `queue_get()`    | Remove from the front    | O(1)       |
| `queue_peek()`   | Look at the front        | O(1)       |
| `free_queue()`   | Free the list            | O(n)       |

## Binary heap - `src/heap`

| Operation         | What it does            | Complexity                  |
| ----------------- | ----------------------- | --------------------------- |
| `create_heap()`   | Create empty heap       | O(1)                        |
| `heap_push()`     | Insert an element       | O(log n)                    |
| `heap_pop()`      | Remove and return root  | O(log n)                    |
| `heap_peek()`     | Look at the root        | O(1)                        |
| `heap_size()`     | Number of elements      | O(1)                        |
| `heap_is_empty()` | Check whether it is empty | O(1)                      |
| `free_heap()`     | Free the buffer         | O(1)                        |

## Hash map - `src/hashmap`

| Operation             | What it does             | Complexity                        |
| --------------------- | ------------------------ | --------------------------------- |
| `create_hashmap()`    | Create empty map         | O(1)                              |
| `map_insert()`        | Insert or update a key   | O(1) average, O(n) worst case     |
| `map_get()`           | Look up a key            | O(1) average, O(n) worst case     |
| `map_remove()`        | Remove a key             | O(1) average, O(n) worst case     |
| `hashmap_hash()`      | Hash a string            | O(m), m = length of the string    |
| `free_hashmap()`      | Free everything          | O(n)                              |

The map uses **separate chaining** with a fixed number of buckets, so a lookup
is O(1) on average and O(n) in the worst case, when every key lands in the
same bucket.

## Hash set - `src/hashset`

| Operation            | What it does             | Complexity                        |
| -------------------- | ------------------------ | --------------------------------- |
| `create_hashset()`   | Create empty set         | O(1)                              |
| `set_add()`          | Add an element           | O(1) average, O(n) worst case     |
| `set_contains()`     | Check membership         | O(1) average, O(n) worst case     |
| `set_remove()`       | Remove an element        | O(1) average, O(n) worst case     |
| `set_hash()`         | Hash a string            | O(m), m = length of the string    |
| `free_hashset()`     | Free everything          | O(n)                              |

The set also uses **separate chaining** with a fixed number of buckets, so a
lookup is O(1) on average and O(n) in the worst case, when every value lands
in the same bucket.

## Graph - `src/graph`

| Operation         | What it does                | Complexity                          |
| ----------------- | --------------------------- | ----------------------------------- |
| `create_graph()`  | Create empty graph          | O(1)                                |
| `add_node()`      | Add a node                  | O(n) - the array is reallocated     |
| `add_edge()`      | Add an edge                 | O(n) - reallocation plus membership |
| `remove_node()`   | Remove a node and its edges | O(n + m)                            |
| `remove_edge()`   | Remove an edge              | O(m)                                |
| `has_node()`      | Check for a node            | O(n)                                |
| `has_edge()`      | Check for an edge           | O(m)                                |
| `path_exists()`   | BFS from src to dest        | O(V * E) with the edge list         |
| `connected()`     | All-pairs reachability      | O(V²) BFS runs -> O(V³ * E)         |
| `print_graph()`   | Print the nodes             | O(n)                                |
| `free_graph()`    | Free everything             | O(n + m)                            |

The graph stores edges in one flat list rather than in adjacency lists, so
finding the neighbours of a node means scanning every edge. That is why the
traversals below are O(V * E) rather than O(V + E).

## Binary tree - `src/trees`

| Operation                   | What it does            | Complexity                  |
| --------------------------- | ----------------------- | --------------------------- |
| `create_btree()`            | Create a root node      | O(1)                        |
| `btree_get_val()`           | Read the value          | O(1)                        |
| `btree_set_val()`           | Overwrite the value     | O(1)                        |
| `btree_get_left/right/parent()` | Read a child or parent | O(1)                    |
| `btree_set_left/right()`    | Attach a child          | O(1)                        |
| `btree_insert_left/right()` | Attach a child with a value | O(1)                   |
| `btree_is_leaf()`           | Check for a leaf        | O(1)                        |
| `btree_height()`            | Height of the tree      | O(n)                        |
| `btree_size()`              | Number of nodes         | O(n)                        |
| `btree_traverse_preorder()` | Root, left, right       | O(n)                        |
| `btree_traverse_inorder()`  | Left, root, right       | O(n)                        |
| `btree_traverse_postorder()`| Left, right, root       | O(n)                        |
| `btree_free()`              | Free the subtree        | O(n)                        |

An **unbalanced** binary tree behaves like a linked list: O(n) for lookups.
A balanced tree (AVL or red-black) keeps the height at O(log n), which turns
lookups into O(log n). This library implements the plain unbalanced tree.

## N-ary tree - `src/trees`

| Operation             | What it does         | Complexity |
| --------------------- | -------------------- | ---------- |
| `create_tree()`       | Create a root node   | O(1)       |
| `tree_add_child()`    | Attach a child       | O(1)       |
| `tree_num_children()` | Count the children   | O(1) - the count is cached |
| `tree_free()`         | Free the subtree     | O(n)       |

## Sorting - `src/algorithms`

| Algorithm                | Best       | Average    | Worst      | Space   | Stable |
| ------------------------ | ---------- | ---------- | ---------- | ------- | ------ |
| `algo_bubble_sort()`     | O(n)       | O(n²)      | O(n²)      | O(1)    | Yes    |
| `algo_selection_sort()`  | O(n²)      | O(n²)      | O(n²)      | O(1)    | No     |
| `algo_insertion_sort()`  | O(n)       | O(n²)      | O(n²)      | O(1)    | Yes    |
| `algo_quick_sort()`      | O(n log n) | O(n log n) | O(n²)      | O(log n)| No     |
| `algo_heap_sort()`       | O(n log n) | O(n log n) | O(n log n) | O(1)    | No     |
| `algo_merge_sort()`      | O(n log n) | O(n log n) | O(n log n) | O(n)    | Yes    |
| `algo_counting_sort_int()` | O(n + k) | O(n + k)   | O(n + k)   | O(k)    | Yes    |

## Searching - `src/algorithms`

| Algorithm                | Requirement | Complexity |
| ------------------------ | ----------- | ---------- |
| `algo_linear_search()`   | Any array   | O(n)       |
| `algo_binary_search()`   | Sorted array| O(log n)   |

## Graph traversal - `src/algorithms`

| Algorithm                   | Finds                            | Complexity           |
| --------------------------- | -------------------------------- | -------------------- |
| `algo_bfs()`                | Every reachable node             | O(V * E)             |
| `algo_dfs()`                | Every reachable node             | O(V * E)             |
| `algo_bfs_shortest_path()`  | The path with the fewest edges   | O(V * E)             |
| `algo_dijkstra()`           | The cheapest path                | O((V + E) log V)     |

The traversals are O(V * E) rather than O(V + E) because the graph stores its
edges in one flat list. `algo_dijkstra()` uses a binary heap as a priority
queue, which is where the `log V` comes from.

---

## Picking the right structure

| You need                        | Use                        |
| ------------------------------- | -------------------------- |
| Fast append, indexed access     | Dynamic array              |
| Insert and remove at the ends   | Doubly linked list         |
| Last in, first out              | Stack                      |
| First in, first out             | Queue                      |
| The smallest/largest element    | Binary heap                |
| Look up by key                  | Hash map                   |
| Test membership by key          | Hash set                   |
| Model relationships             | Graph                      |
| Hierarchical data               | Tree                       |
| Sort once, look up many times   | Sort, then binary search   |

## See also

- [Explanations](../explainations/README.en.md) - a walkthrough of every structure
