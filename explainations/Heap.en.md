# Binary Heap - Explained

## What is a binary heap?

A binary heap is a **complete binary tree** that is stored in a flat array. The
"complete" part means every level is filled up completely except possibly the
last one, which is filled from left to right. Because of that shape, a heap can
be stored in an array without any pointers at all.

The defining property is the **heap order**: every node is ordered against its
parent. In a *min-heap* the parent is never larger than its children, so the
smallest element of the whole heap sits at the root. In a *max-heap* the parent
is never smaller, and the largest element sits at the root.

The children of the element at index `i` live at index `2 * i + 1` and
`2 * i + 2`. That arithmetic is the whole trick: it is why heaps have no
pointer overhead and why they cache beautifully.

A heap is **not** sorted. It only guarantees that the root is extreme. You get
the elements out in sorted order by repeatedly removing the root and restoring
the heap property - that is exactly what heap sort does.

## Implementation details

- **Structure**: `struct Heap { void **items; size_t size; size_t capacity; cmp_fn cmp; }`
- **Order is decided by the comparator**, not hard-coded. With a comparator
  that puts the smaller value first you get a min-heap; flip the comparator and
  the exact same code becomes a max-heap. This is why `create_heap()` takes a
  `cmp_fn` (the same `int (*)(const void *, const void *)` signature that
  `qsort()` uses).
- **The array is 0-indexed and the root is `items[0]`.**
- `create_heap(cmp)` allocates the heap with an initial capacity and stores the
  comparator. It returns `NULL` if the comparator is `NULL` or the allocation
  fails.
- `heap_push()` appends the element at the end and then **sifts it up**: it
  swaps with its parent for as long as the parent is "greater" according to the
  comparator. The tree is complete at all times, so the element can always be
  appended without shifting anything.
- `heap_pop()` takes the root, moves the last element into its place, shrinks
  the size by one and then **sifts that element down**: it swaps with whichever
  child should come first until the heap order is restored again.
- `heap_peek()` returns `items[0]` without removing it.
- `heap_size()` and `heap_is_empty()` are trivial accessors.
- `free_heap()` frees the internal buffer and the heap itself. It is NULL-safe.
- **Payloads are owned by the caller.** The heap stores `void *` pointers and
  never copies or frees what they point to.
- The buffer **grows by doubling**, which keeps the amortized cost of a push
  at O(1).

## Operations (time complexity)

| Operation       | What it does                        | Time Complexity            |
| --------------- | ----------------------------------- | -------------------------- |
| `create_heap()` | Create empty heap                   | O(1)                       |
| `heap_push()`   | Insert an element                   | O(log n) amortized         |
| `heap_pop()`    | Remove and return the root          | O(log n)                   |
| `heap_peek()`   | View the root without removing it   | O(1)                       |
| `heap_size()`   | Number of elements                  | O(1)                       |
| `heap_is_empty()` | Check whether it holds no elements | O(1)                       |
| `free_heap()`   | Free buffer and heap                | O(1)                       |

## Pros

- O(log n) insert and extract-min, which is the point of the structure
- No pointers - just one flat array, so it is very cache friendly
- Doubling growth gives amortized O(1) push
- One comparator flips it between min-heap and max-heap
- Can be built on top of any other array-like container

## Cons

- Not sorted - only the root is guaranteed to be extreme
- No random access by value
- Only the root can be removed
- Slower than a plain array for repeated append-only work
- `heap_pop()` is not stable, so it cannot preserve the order of equal elements

## When to use

- Priority queues and job schedulers ("process the most urgent task first")
- Dijkstra's algorithm (see [Algorithms](Algorithms.en.md))
- Heap sort
- "Top k" problems: k largest or k smallest elements out of a stream
- Event simulation and A* path finding
- Merge k sorted streams

## Example

```c
#include "heap/heap.h"

// Orders two integers ascending. Negating the comparison turns the very
// same heap into a max-heap.
static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

int main() {
    struct Heap *heap = create_heap(cmp_int);

    int values[] = {5, 1, 8, 3};
    for (int i = 0; i < 4; i++) {
        heap_push(heap, &values[i]);
    }

    printf("smallest: %d\n", *(int *)heap_peek(heap));

    // Popping in a loop yields the elements in ascending order.
    while (!heap_is_empty(heap)) {
        printf("%d ", *(int *)heap_pop(heap));
    }
    printf("\n");  // 1 3 5 8

    free_heap(heap);
    return 0;
}
```

## See also

- [Algorithms](Algorithms.en.md) - heap sort and Dijkstra are built on this
- [Complexity cheat sheet](../docs/COMPLEXITY.md)
