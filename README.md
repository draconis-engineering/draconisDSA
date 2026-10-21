# DraconisDSA

**Data Structures & Algorithms in C23 - a teaching reference you can read,
teach from, or copy straight into your own project.**

No dependencies. No framework. Every structure lives in its own folder with
its own header, and every one of them is covered by tests.

```c
#include "algorithms/algorithms.h"
#include "heap/heap.h"

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void) {
    // A min-heap, because the comparator puts the smaller value first.
    struct Heap *heap = create_heap(cmp_int);

    int values[] = {5, 1, 8, 3};
    for (int i = 0; i < 4; i++) {
        heap_push(heap, &values[i]);
    }

    while (!heap_is_empty(heap)) {
        printf("%d ", *(int *)heap_pop(heap));
    }
    printf("\n");  // 1 3 5 8

    free_heap(heap);
    return 0;
}
```

## What is in here

### Data structures

| Module        | What it is                                                 |
| ------------- | ---------------------------------------------------------- |
| `src/array`   | Dynamic array with doubling growth                         |
| `src/sllist`  | Singly linked list                                         |
| `src/dllist`  | Doubly linked list                                         |
| `src/stack`   | LIFO stack, built on the doubly linked list                |
| `src/queue`   | FIFO queue, built on the doubly linked list                |
| `src/heap`    | Binary heap, ordered by a comparator you supply            |
| `src/hashmap` | String-keyed hash map (DJB2 + separate chaining)           |
| `src/hashset` | Hash set of unique strings (DJB2 + separate chaining)      |
| `src/graph`   | Directed weighted graph                                    |
| `src/trees`   | Binary tree and n-ary tree with the three traversal orders |

### Algorithms

| Module                     | What is in it                                                   |
| -------------------------- | --------------------------------------------------------------- |
| `src/algorithms/sort`      | Bubble, selection, insertion, quick, heap, merge, counting sort |
| `src/algorithms/search`    | Linear search and binary search                                 |
| `src/algorithms/traversal` | BFS, DFS, BFS shortest path and Dijkstra's algorithm            |

Every generic algorithm is driven by a comparator with the same signature as
`qsort()`:

```c
typedef int (*cmp_fn)(const void *a, const void *b);
```

Return a negative number if `a` sorts before `b`, zero if they are equal, and
a positive number if `a` sorts after `b`.

## Building

```sh
make          # build the library and the demo
./main.exe    # run the demo, which exercises every structure
make clean    # remove the build artefacts
```

## Testing

```sh
make test           # build and run the test suite
make test-sanitize  # the same tests under ASan and UBSan
```

The suite has no external dependencies and lives in `tests/`. See
[tests/README.md](tests/README.md) for how to write a test case.

## Documentation

| Where                                        | What it holds                                  |
| -------------------------------------------- | ---------------------------------------------- |
| [explainations/](explainations/README.en.md) | A walkthrough of every structure and algorithm |
| [docs/COMPLEXITY.md](docs/COMPLEXITY.md)     | The time complexity of every public operation  |
| [docs/README.md](docs/README.md)             | The API reference and the rules of the library |
| [docs/ROADMAP.md](docs/ROADMAP.md)           | What is planned next                           |
| [docs/CHANGELOG.md](docs/CHANGELOG.md)       | What changed                                   |

## The rules of the library

1. **One folder per module.** Each structure gets its own header and
   translation unit, so you can pick up exactly one file at a time.
2. **`create_*` and `free_*` go together.** Everything allocated has a
   matching release function, and the release functions are NULL-safe.
3. **`void *` payloads, caller-owned.** The containers store pointers and
   never free what those pointers point at. The exceptions are documented:
   the hash set owns its key strings and the hash map owns its keys.
4. **No allocation failure is ignored.** Every allocator call is checked and
   reported.
5. **Invalid arguments are handled, not undefined.** Out-of-range indices,
   empty pops and NULL pointers all have a defined outcome.
6. **Everything is documented.** Every public function has a Doxygen comment
   that says what it does, including what happens when the arguments are
   invalid.

## License

GPL-3.0. See [LICENSE](LICENSE).
