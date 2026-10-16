# Queue - Explained

## What is a queue?

A queue is a FIFO (First In, First Out) data structure. Elements are added to the rear (enqueue) and removed from the front (dequeue). This implementation is built on top of `DLList`, storing `void *data` payloads with caller ownership.

## Implementation details

- **Structure**: `struct Queue { struct DLList *entries; size_t size; }`
- `create_queue()` allocates queue and internal `DLList`.
- `queue_add()` inserts at the back of the underlying list (`dll_insert_at_back`) and increments `size`.
- `queue_get()` returns `entries->head->data` if not empty, removes element at index 0 (`dll_remove(entries, 0)`), decrements `size`. Returns `NULL` if empty.
- `queue_peek()` returns head data without removing, or `NULL` if empty.
- `free_queue()` frees underlying list and queue.

## Operations (time complexity)

| Operation | What it does | Time Complexity |
|-----------|--------------|-----------------|
| `create_queue()` | Create empty queue | O(1) |
| `queue_add()` (enqueue) | Add to rear | O(1) (DLL back) |
| `queue_get()` (dequeue) | Remove from front | O(1) for DLL front removal |
| `queue_peek()` | View front without removing | O(1) |
| `free_queue()` | Free queue and entries | O(n) |

## Pros

- O(1) enqueue/dequeue/peek
- Clear FIFO semantics
- Dynamic size
- Simple interface

## Cons

- No random access
- Only front/back accessible via API

## When to use

- Breadth-first search (BFS)
- Task scheduling, job queues
- Buffering/streaming
- Event handling systems

## Example

```c
#include "queue.h"

int main() {
    struct Queue *q = create_queue();
    int a = 1, b = 2;
    queue_add(q, &a);
    queue_add(q, &b);
    int *front = (int*)queue_peek(q);  // a
    int *deq = (int*)queue_get(q);     // a
    free_queue(q);
    return 0;
}
```
