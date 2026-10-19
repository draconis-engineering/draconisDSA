# Dynamic Array - Explained

## What is a dynamic array?

A dynamic array is a contiguous, resizable sequence of elements. Unlike a fixed-size C array, it can grow as you append to it. This implementation stores `void *` payloads in a heap-allocated buffer and doubles that buffer's capacity whenever it runs out of room, which keeps appends cheap on average (amortized O(1)).

It is the closest thing to a "C++ `std::vector`" or a "Python `list`" in this library.

## Implementation details

- **Structure**: `struct Array { unsigned int size; unsigned int capacity; void *data; }`
- `data` points to a contiguous buffer of `void*` slots (`capacity` of them); `size` tracks how many are in use.
- `create_array()` allocates the `Array` plus an initial buffer of `ARRAY_INITIAL_CAPACITY` (4) slots.
- Payloads are `void*` and owned by the caller (only the buffer is freed on `free_array`).
- When the buffer is full, `arr_push()`/`arr_insert()` call the internal `arr_grow()`, which `realloc`s the buffer to **double** its capacity.
- `arr_insert()` at `idx` shifts elements `idx..size-1` one slot to the right; `arr_remove()` shifts `idx+1..size-1` one slot to the left. Both preserve order.
- The implementation performs bounds checking and prints errors to stdout on invalid indices or allocation failures.
- Insertion indices: `0..size`. Access/removal indices: `0..size-1`.

## Operations (time complexity)

| Operation        | What it does                           | Time Complexity                |
| ---------------- | -------------------------------------- | ------------------------------ |
| `create_array()` | Create empty array (buffer of 4)       | O(1)                           |
| `arr_size()`     | Return number of elements              | O(1)                           |
| `arr_is_empty()` | Check if empty                         | O(1)                           |
| `arr_push()`     | Append at back (grows if full)         | O(1) amortized, O(n) on growth |
| `arr_insert()`   | Insert at index `0..size`, shift right | O(n)                           |
| `arr_get()`      | Read element by index                  | O(1)                           |
| `arr_set()`      | Overwrite element by index             | O(1)                           |
| `arr_remove()`   | Remove at index, shift left            | O(n)                           |
| `arr_pop()`      | Remove and return last element         | O(1)                           |
| `print_array()`  | Print pointer values                   | O(n)                           |
| `free_array()`   | Free buffer and array                  | O(1)                           |

## Pros

- O(1) random access by index (contiguous memory, cache-friendly)
- Amortized O(1) append at the back
- Dynamic size; grows automatically
- Less memory per element than a linked list (no per-node pointers)

## Cons

- Insertion/removal in the middle is O(n) due to shifting
- Growth (reallocation) occasionally costs O(n) and may invalidate stored pointers into the buffer
- No shrink-on-remove; capacity is never reduced automatically
- Unused capacity can waste memory after many removals

## When to use

- When you need fast indexing by position
- When most inserts happen at the end (append-heavy workloads)
- As a backing store for stacks, heaps, hash tables, and symbol tables
- When cache locality matters more than cheap middle insertions

## Example

```c
#include "array.h"

int main() {
    struct Array *a = create_array();
    int x = 1, y = 2, z = 3;
    arr_push(a, &x);           // [x]
    arr_push(a, &y);           // [x, y]
    arr_insert(a, &z, 1);      // [x, z, y]
    int *second = (int*)arr_get(a, 1);  // z
    arr_pop(a);                // [x, z]
    arr_remove(a, 0);          // [z]
    free_array(a);
    return 0;
}
```
