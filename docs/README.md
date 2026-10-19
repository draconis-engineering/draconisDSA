# DraconisDSA

Data Structures & Algorithms library written in C23. Educational, dependency-free, and split into one small module per structure (each with its own header and translation unit).

## Structures

| Module        | Description                                                      |
| ------------- | ---------------------------------------------------------------- |
| `src/array`   | Dynamic array (`arr_`-prefixed API), doubling growth             |
| `src/sllist`  | Singly linked list (`sll_`-prefixed API)                         |
| `src/dllist`  | Doubly linked list (`dll_`-prefixed API)                         |
| `src/stack`   | LIFO stack built on `dllist`                                     |
| `src/queue`   | FIFO queue built on `dllist`                                     |
| `src/hashmap` | String-keyed hash map, DJB2 hash + separate chaining             |
| `src/hashset` | Hash set of unique strings, DJB2 hash + separate chaining        |
| `src/graph`   | Directed weighted graph (BFS `path_exists`, connectivity)        |
| `src/trees`   | Binary tree (`btree_`) + n-ary tree (`tree_`), pre/in/post-order |

## Building

Use `make` to compile the library and demo.

```sh
make
```

`src/main.c` is a small demo that exercises most structures (the hash set is not shown there; see `examples` in `explainations/HashSet.en.md`).

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

## Memory

Everything created with `create_*` must be released with `free_*` (e.g. `free_sllist`).

## License

GPL-3.0. See [LICENSE](../LICENSE).
