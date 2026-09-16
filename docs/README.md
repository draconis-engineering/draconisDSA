# DraconisDSA

Data Structures & Algorithms library written in C99. Educational, single-header, and dependency-free.

## Structures

| Module | Description |
| ------ | ----------- |
| `src/sllist` | Singly linked list (`sll_`-prefixed API) |
| `src/dllist` | Doubly linked list (`dll_`-prefixed API) |
| `src/stack` | LIFO stack built on `dllist` |
| `src/queue` | FIFO queue built on `dllist` |
| `src/hashmap` | String-keyed hash map, DJB2 hash + separate chaining |
| `src/graph` | Directed weighted graph (BFS `path_exists`, connectivity) |

## Building

There is no build system yet; compile manually. For example:

```sh
gcc -std=c99 -Wall -Wextra -Isrc \
    src/main.c \
    src/sllist/sllist.c src/dllist/dllist.c \
    src/stack/stack.c src/queue/queue.c \
    src/hashmap/hashmap.c src/graph/graph.c \
    -o draconis_dsa
```

`src/main.c` is a small demo that exercises every structure.

## Usage

Each module lives in its own folder and exposes its types in the matching header.
Structures store `void *` payloads; ownership of the payloads stays with the caller
(only the container's nodes are freed).

Lists accept indices `0..size` for insertion and `0..size-1` for removal.
Every allocator failure is handled and reported; empty-pop/peek and empty-dequeue
return `NULL`.

## Memory

Everything created with `create_*` must be released:

- `free_sllist` / `free_list` — free nodes only; you must `free()` the list struct itself.
- `free_stack` / `free_queue` — free the whole container.
- `free_hashmap` — frees keys, nodes, and the map.
- `free_graph` — frees nodes, edges, and the graph.

## License

GPL-3.0. See [LICENSE](LICENSE).