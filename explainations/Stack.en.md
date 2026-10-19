# Stack - Explained

## What is a stack?

A stack is a LIFO (Last In, First Out) data structure. Elements are pushed onto the top and popped from the top. This implementation of `Stack` is built on top of `DLList` (doubly linked list). The stack stores `void *data` payloads; ownership remains with the caller.

## Implementation details

- **Structure**: `struct Stack { struct DLList *entries; }` (the element count lives in `entries->size`)
- `create_stack()` allocates both the `Stack` and an internal `DLList` via `create_dll()`. On failure of `create_dll()`, the stack is freed.
- `stack_push()` inserts at the back of the underlying list (`dll_insert_at_back`).
- `stack_pop()` returns `entries->tail->data` if not empty, then removes the last element (`dll_remove` at `entries->size - 1`). Returns `NULL` if empty.
- `stack_peek()` returns the tail data without removing it, or `NULL` if empty.
- `free_stack()` frees the underlying list and the stack itself.

## Operations (time complexity)

| Operation        | What it does                      | Time Complexity              |
| ---------------- | --------------------------------- | ---------------------------- |
| `create_stack()` | Create empty stack                | O(1)                         |
| `stack_push()`   | Push element onto top             | O(1) (DLL back insertion)    |
| `stack_pop()`    | Pop element from top              | O(1) (removes from DLL tail) |
| `stack_peek()`   | View top without removing         | O(1)                         |
| `free_stack()`   | Free stack and underlying entries | O(n) (freed by `free_dll`)   |

## Pros

- Simple, intuitive LIFO behavior
- O(1) push/pop/peek
- Built on reusable DLL component
- Memory grows dynamically

## Cons

- Limited access (only top)
- No random access or iteration by index through stack API

## When to use

- Function call stacks, recursion tracking
- Undo/redo functionality
- Expression evaluation (postfix/infix)
- Backtracking algorithms

## Example

```c
#include "stack.h"

int main() {
    struct Stack *s = create_stack();
    int a = 1, b = 2;
    stack_push(s, &a);
    stack_push(s, &b);
    int *top = (int*)stack_peek(s);  // b
    int *popped = (int*)stack_pop(s); // b
    free_stack(s);
    return 0;
}
```
