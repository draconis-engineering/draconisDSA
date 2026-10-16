# HashSet - Explained

## What is a hash set?

A hash set stores unique values (no duplicates). This implementation stores string values using separate chaining for collisions, with DJB2 hashing. It supports add, contains, remove, and cleanup.

## Implementation details

- **Node**: `struct Node { char *value; struct Node *next; }` (note: same struct name as HashMap's node, but in different compilation unit context)
- **Set**: `struct HashSet { struct Node **buckets; size_t capacity; size_t size; }`
- `INITIAL_CAPACITY` is `16`
- `hashset_hash()` computes DJB2 hash and modulates by `set->capacity`
- `hashset_contains()` searches bucket for matching string (`strcmp`); returns `true` if found
- `hashset_add()` returns `false` if `set/value` is NULL, if already contains value (sets do not allow duplicates), or on allocation failure. Otherwise duplicates string with `strdup()`, inserts at head of bucket, increments `size`, returns `true`
- `hashset_remove()` finds and removes matching node, frees value and node, decrements size; returns `false` if not found
- `hashset_destroy()` frees all nodes, values, buckets, and set

## Operations (time complexity)

| Operation | What it does | Time Complexity |
|-----------|--------------|-----------------|
| `create_hashset()` | Create empty set | O(C) where C=capacity (16) - zeroes buckets |
| `hashset_hash()` | Compute index | O(k) where k = string length |
| `hashset_contains()` | Check membership | O(k + m) average, O(n) worst case |
| `hashset_add()` | Add unique value | O(k + m) average (includes contains check), O(n) worst case |
| `hashset_remove()` | Remove value | O(k + m) average, O(n) worst case |
| `hashset_destroy()` | Free all memory | O(n + C) |

## Pros

- Fast membership tests on average
- Enforces uniqueness
- Simple API

## Cons

- No dynamic resizing (fixed initial capacity)
- Only supports string values
- Same struct name `Node` as hashmap - could be confusing if used together; better to use distinct names in larger codebases
- `strdup` is POSIX (used directly) - fine on most platforms, though codebase also uses custom `dup_string` in hashmap for portability

## When to use

- When you need to test membership quickly
- To deduplicate collections of strings
- When order doesn't matter

## Example

```c
#include "hashset.h"

int main() {
    struct HashSet *s = create_hashset();
    hashset_add(s, "apple");
    hashset_add(s, "banana");
    bool has = hashset_contains(s, "apple"); // true
    hashset_remove(s, "apple");
    hashset_destroy(s);
    return 0;
}
```
