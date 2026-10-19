# Singly Linked List - Explained

## What is a singly linked list?

A singly linked list (SLL) is an ordered data structure consisting of nodes (`SLLNode`) that each store a payload (`void *data`) and a single pointer (`next`) to the next node in the sequence. The list container (`SLList`) keeps track of the `head` (first node), `tail` (last node), and the `size` (number of nodes).

Unlike arrays, nodes in a singly linked list are not stored in contiguous memory. Each node is allocated separately on the heap.

## Implementation details

- **Node**: `struct SLLNode { void *data; struct SLLNode *next; }`
- **List**: `struct SLList { int size; struct SLLNode *head; struct SLLNode *tail; }`
- The list supports storing arbitrary data via `void*` pointers; the caller owns the payloads (only nodes are freed).
- Operations allow insertion at index `0..size` and removal at index `0..size-1` (matching the API behavior). Passing `-1` to `sll_insert` inserts at the back.
- Empty list: `head == NULL` and `tail == NULL`, `size == 0`.

## Operations (time complexity)

| Operation               | What it does                                       | Time Complexity                          |
| ----------------------- | -------------------------------------------------- | ---------------------------------------- |
| `create_sll()`          | Creates an empty list                              | O(1)                                     |
| `sll_is_empty()`        | Checks if empty                                    | O(1)                                     |
| `sll_size()`            | Returns size                                       | O(1)                                     |
| `sll_get_idx()`         | Finds first index of a value (by pointer equality) | O(n)                                     |
| `sll_insert_at_front()` | Insert at front                                    | O(1)                                     |
| `sll_insert_at_back()`  | Insert at back                                     | O(1)                                     |
| `sll_insert()`          | Insert at arbitrary index (0..size, -1 = back)     | O(n) - needs traversal to reach position |
| `sll_remove()`          | Remove at index (0..size-1)                        | O(n) - traversal needed except index 0   |
| `print_sll()`           | Print pointer values                               | O(n)                                     |
| `free_sll()`            | Free all nodes and list structure                  | O(n)                                     |

_Note:_ `sll_get_idx()` compares pointers (`curr_node->data == val`), not the contents of the data.

## Pros

- Dynamic size (grows/shrinks as needed)
- O(1) insertion/deletion at head and tail
- No need to pre-allocate memory
- Easy to implement and understand

## Cons

- No random access (must traverse to reach index k)
- Extra memory overhead per node (the `next` pointer)
- Traversal is one-directional only (cannot go backwards)
- Not cache-friendly (non-contiguous memory)

## When to use

- When you need fast insertions/deletions at the front/back
- When size is unknown and varies frequently
- When you don't need random indexing
- When memory allocation overhead is acceptable

## Example usage

```c
#include "sllist.h"

int main() {
    struct SLList *l = create_sll();
    int x = 10, y = 20, z = 30;
    sll_insert_at_back(l, &x);
    sll_insert_at_front(l, &y);  // [y, x]
    sll_insert(l, &z, 1);        // [y, z, x]
    print_sll(l);
    free_sll(l);
    return 0;
}
```
