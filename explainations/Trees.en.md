# Trees - Explained

## What are trees?

This library provides two tree types: a binary tree (`BinaryTree`) and an n-ary (general) tree (`Tree`). Both store `void *val` payloads.

### Binary Tree

A binary tree node has left, right, and parent pointers. The API supports getters/setters, insertion of children (which replaces and frees any existing subtree), traversal (pre/in/post-order), and basic properties (leaf, height, size).

### N-ary Tree

An n-ary tree stores children in a growable array (`struct Tree **children`), with `num_children` and `capacity`. Children are appended dynamically.

## Implementation details

### Binary Tree

- **Node**: `struct BinaryTree { void *val; struct BinaryTree *parent; struct BinaryTree *left; struct BinaryTree *right; }`
- `create_btree(val)` creates a leaf node with NULL parent/children
- `btree_free(tree)` recursively frees left subtree, right subtree, then node itself (postorder deletion). NULL-safe.
- Getters (`btree_get_val/left/right/parent`) return NULL if node is NULL
- Setters: `btree_set_val` replaces payload. `btree_set_left/right` free any previously attached subtree and wire child's parent pointer; if `child == tree->left/right` already, returns early (no-op). If setting to NULL, frees old subtree and sets pointer to NULL.
- `btree_insert_left/right` creates a new node with `val`, attaches via setters (freeing old subtree), returns new node or NULL on failure
- `btree_is_leaf` returns true only if node != NULL and left==NULL and right==NULL
- `btree_height`: height defined as number of edges on longest downward path to leaf. Leaf has height 0; NULL has height -1. (Standard definition used here.)
- `btree_size`: total nodes in subtree (1 + size(left) + size(right)), 0 if NULL
- Traversals are recursive DFS: preorder (root,left,right), inorder (left,root,right), postorder (left,right,root). `visit(void*)` is called per node’s payload.

### N-ary Tree

- **Node**: `struct Tree { void *val; int num_children; int capacity; struct Tree **children; }`
- `create_tree(val)` creates node with empty children array (NULL, capacity 0)
- `tree_free(tree)` recursively frees each child subtree, then frees `children` array and node
- `tree_add_child(parent, child_val)`: if at capacity, grows array (starts at 4, doubles). Creates new child node; on failure returns NULL. Appends to array, increments count, returns child
- `tree_num_children(tree)` returns count or 0 if NULL

## Operations (time complexity)

### Binary Tree

| Operation | Time Complexity | Notes |
|-----------|-----------------|-------|
| `create_btree` | O(1) | |
| `btree_free` | O(n) | visits every node once |
| Getters/Setters (non-freeing) | O(1) | |
| `btree_set_left/right` (with free) | O(size of old subtree) | recursive free of old subtree |
| `btree_insert_left/right` | O(size of old subtree) | creates node O(1) + set_left/right |
| `btree_is_leaf` | O(1) | |
| `btree_height` | O(n) | visits every node (tree not necessarily balanced) |
| `btree_size` | O(n) | visits every node |
| Traversals (pre/in/postorder) | O(n) | visits each node once |

### N-ary Tree

| Operation | Time Complexity | Notes |
|-----------|-----------------|-------|
| `create_tree` | O(1) | |
| `tree_free` | O(n) | total nodes n |
| `tree_add_child` | O(1) amortized | array append with doubling |
| `tree_num_children` | O(1) | |

## Pros

- Clean recursive implementations (easy to understand)
- Binary tree includes parent pointers (useful for some operations)
- N-ary tree uses dynamic array for children (good cache locality vs linked lists)
- Setters properly manage memory (free old subtrees) to avoid leaks

## Cons

- Recursive DFS/traversals and frees can overflow stack on deep trees (skewed trees)
- No balancing (BST not implemented here - just binary tree structure)
- N-ary tree has no way to remove children or traverse (beyond freeing whole subtrees)
- Binary tree insertion overwrites existing children by freeing them (destructive behavior) - intentional per API

## When to use

- Learning tree terminology and traversals
- Building expression trees, parse trees (small)
- General hierarchical data with variable branching factor (n-ary)

## Example

```c
#include "trees.h"

static void print_int(void *v) {
    if (v) printf("%d ", *(int*)v);
}

int main() {
    int v1=1, v2=2, v3=3;
    struct BinaryTree *root = create_btree(&v1);
    btree_insert_left(root, &v2);
    btree_insert_right(root, &v3);
    btree_traverse_inorder(root, print_int); // prints 2 1 3
    btree_free(root);

    struct Tree *t = create_tree(&v1);
    tree_add_child(t, &v2);
    tree_add_child(t, &v3);
    tree_free(t);
    return 0;
}
```
