# Doubly Linked List - Explained

## What is a doubly linked list?

A doubly linked list (DLL) is an ordered data structure where each node (`DLLNode`) stores a payload (`void *data`), a pointer to the `next` node, and a pointer to the `prev` node. The container (`DLList`) tracks `head`, `tail`, and `size`. This bidirectional linking makes it easier to traverse both ways and to remove from either end efficiently.

## The shape of the list

A list holding the payloads `10`, `20`, `30` in order. Every node carries two
arrows, so any node that is in hand knows both of its neighbours:

```
 head                                        tail
  |                                          |
  v                                          v
+-------+          +-------+          +-------+
|  10   |  next    |  20   |  next    |  30   |
|       |--------->|       |--------->|       |
|       |<---------|       |<---------|       |
+-------+  prev    +-------+  prev    +-------+

   size: 3        (the ends are NULL: 10.prev and 30.next)
```

The payoff of the second arrow is removing a middle node without any walking:
two pointer updates splice the node out.

```
remove [20]:

before:   [10] <--> [20] <--> [30]

          [10].next ---------> [30]     (1)
          [30].prev ---------> [10]     (2)
          free [20]

after:    [10] <--> [30]
```

Both ends are symmetric: `dll_remove(l, 0)` and `dll_remove(l, size - 1)`
are O(1) because the list holds `head` and `tail` node pointers directly.

## Implementation details

- **Node**: `struct DLLNode { void *data; struct DLLNode *next; struct DLLNode *prev; }`
- **List**: `struct DLList { int size; struct DLLNode *head; struct DLLNode *tail; }`
- Payloads are `void*` and owned by the caller (only nodes freed on `free_dll`).
- Insertion indices: `0..size` (or `-1` for back). Removal indices: `0..size-1` (as implemented).
- The implementation performs bounds checking and prints errors to stdout on invalid indices or allocation failures.

## Operations (time complexity)

| Operation               | What it does                                 | Time Complexity                     |
| ----------------------- | -------------------------------------------- | ----------------------------------- |
| `create_dll()`          | Create empty list                            | O(1)                                |
| `dll_is_empty()`        | Check if empty (checks `head == NULL`)       | O(1)                                |
| `dll_size()`            | Return size                                  | O(1)                                |
| `dll_get_idx()`         | Find first index of value (pointer equality) | O(n)                                |
| `dll_insert_at_front()` | Insert at front                              | O(1)                                |
| `dll_insert_at_back()`  | Insert at back                               | O(1)                                |
| `dll_insert()`          | Insert at index (0..size, -1=back)           | O(n) - traverses from head to index |
| `dll_remove()`          | Remove at index (0..size-1)                  | O(1) at ends, O(n) in middle        |
| `print_dll()`           | Print pointer values                         | O(n)                                |
| `free_dll()`            | Free all nodes and list                      | O(n)                                |

## Pros

- O(1) insertion/deletion at both ends
- Can traverse forwards and backwards (if extended)
- Easier to remove a node if you have a pointer to it (common pattern)
- Dynamic size

## Cons

- More memory per node (extra `prev` pointer) compared to an SLL
- Slightly more complex insertion/deletion logic
- Still no random access; traversal needed for arbitrary index
- Non-contiguous memory

## When to use

- When you need efficient operations at both ends (e.g. deque)
- When you need to undo/traverse backwards
- When you expect frequent removals from ends

## Example

```c
#include "dllist.h"

int main() {
    struct DLList *l = create_dll();
    int a = 1, b = 2, c = 3;
    dll_insert_at_back(l, &a);
    dll_insert_at_front(l, &b);  // [b, a]
    dll_insert(l, &c, 1);        // [b, c, a]
    dll_remove(l, 0);            // [c, a]
    free_dll(l);
    return 0;
}
```
