# HashMap - Explained

## What is a hash map?

A hash map (dictionary/associative array) maps string keys to integer values. This implementation uses separate chaining for collision resolution: each bucket is a linked list (`struct Node`) of entries with the same hash. It uses the DJB2 hash function.

## Implementation details

- **Entry node**: `struct Node { char *key; int value; struct Node *next; }`
- **Map**: `struct HashMap { struct Node *buckets[BUCKETS]; }` where `BUCKETS` is `16` (defined in header)
- Hash function: DJB2 (`hash = ((v<<5)+v) + key[i]`, then `v % BUCKETS`)
- Keys are duplicated with a portable `dup_string()` (allocates and copies); owned by the map
- `map_insert()` checks for existing key first - if found, updates value; otherwise prepends a new node to the bucket (O(1) insertion at head)
- `map_get()` searches bucket chain for matching key; returns value and sets `*found = 1` if found, otherwise sets `*found = 0` and returns `-1`
- `map_remove()` finds and removes entry from chain (handles head case), frees key and node
- `free_hashmap()` frees all nodes, all duplicated keys, and the map

## Operations (time complexity)

| Operation          | What it does                    | Time Complexity                                                                  |
| ------------------ | ------------------------------- | -------------------------------------------------------------------------------- |
| `create_hashmap()` | Create empty map                | O(B) where B=BUCKETS (16) - initializes buckets                                  |
| `hash()`           | Compute hash index for key      | O(k) where k = key length                                                        |
| `map_insert()`     | Insert or update key-value pair | O(k + m) average; O(n) worst case if all collide in one bucket (n entries total) |
| `map_get()`        | Retrieve value for key          | O(k + m) average, O(n) worst case                                                |
| `map_remove()`     | Remove key if present           | O(k + m) average, O(n) worst case                                                |
| `free_hashmap()`   | Free all allocated memory       | O(n + B)                                                                         |

## Pros

- Fast average lookups/inserts/deletes for string keys
- Simple collision handling (separate chaining)
- Keys stored as copies for ownership safety

## Cons

- Fixed bucket count (16) - no dynamic resizing (can degrade to linked list if many entries)
- Uses pointer equality not applicable; keys compared by string content (`strcmp`)
- Returns `-1` as sentinel for "not found" which conflicts if `-1` is a valid value (caller must use `found` flag)
- Only supports `int` values (not generic `void*` like lists)

## When to use

- When mapping string keys to integer values
- For small-to-medium datasets with reasonable key distribution
- When simplicity is preferred over advanced features (rehashing, load factor)

## Example

```c
#include "hashmap.h"

int main() {
    struct HashMap *m = create_hashmap();
    map_insert(m, "foo", 42);
    map_insert(m, "bar", 7);
    int found;
    int v = map_get(m, "foo", &found); // found=1, v=42
    map_remove(m, "foo");
    free_hashmap(m);
    return 0;
}
```
