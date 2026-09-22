/*
 * Tree Library - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
 * Copyright (C) 2026 Simon Stordal Amundgård
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses.
 */

#ifndef TREES_H
#define TREES_H

#include <stdbool.h>
#include <stddef.h>

// ================== //
// -- BINARY TREES -- //
// ================== //

struct BinaryTree {
	void *val;
	struct BinaryTree *parent;
	struct BinaryTree *left;
	struct BinaryTree *right;
};

// Creates a new node holding `val` with no children and no parent.
struct BinaryTree *btree_create(void *val);

// Recursively frees the node and all descendants. NULL-safe.
void btree_free(struct BinaryTree *tree);

// Getters. All are NULL-safe and return NULL for a missing node/value.
void *btree_get_val(const struct BinaryTree *tree);
struct BinaryTree *btree_get_left(const struct BinaryTree *tree);
struct BinaryTree *btree_get_right(const struct BinaryTree *tree);
struct BinaryTree *btree_get_parent(const struct BinaryTree *tree);

// Replaces the node's payload.
void btree_set_val(struct BinaryTree *tree, void *val);

// Replaces the given child, recursively freeing any previously attached
// subtree, and wires the new child's parent pointer. NULL-safe.
void btree_set_left(struct BinaryTree *tree, struct BinaryTree *child);
void btree_set_right(struct BinaryTree *tree, struct BinaryTree *child);

// Creates a new node with `val`, attaches it as the given child (replacing and
// freeing any previous subtree), and returns the new node (NULL on failure).
struct BinaryTree *btree_insert_left(struct BinaryTree *tree, void *val);
struct BinaryTree *btree_insert_right(struct BinaryTree *tree, void *val);

// A node with no children.
bool btree_is_leaf(const struct BinaryTree *tree);

// Number of edges on the longest downward path to a leaf.
// Leaf = 0, NULL = -1, empty-op is not an error.
int btree_height(const struct BinaryTree *tree);

// Total number of nodes in the subtree rooted at `tree`.
int btree_size(const struct BinaryTree *tree);

// Depth-first traversals. `visit` is called with each node's payload.
void btree_traverse_preorder(const struct BinaryTree *tree,
							 void (*visit)(void *));
void btree_traverse_inorder(const struct BinaryTree *tree,
							void (*visit)(void *));
void btree_traverse_postorder(const struct BinaryTree *tree,
							  void (*visit)(void *));

// =================== //
// -- REGULAR TREES -- //
// =================== //

// An n-ary tree. Children are stored in a growable array.
struct Tree {
	void *val;
	int num_children;
	int capacity;
	struct Tree **children;
};

// Creates a new n-ary tree node holding `val`.
struct Tree *tree_create(void *val);

// Recursively frees the node and all descendants. NULL-safe.
void tree_free(struct Tree *tree);

// Appends a new child holding `child_val` and returns it (NULL on failure).
struct Tree *tree_add_child(struct Tree *parent, void *child_val);

// Number of children currently attached to the node.
int tree_num_children(const struct Tree *tree);

#endif // TREES_H