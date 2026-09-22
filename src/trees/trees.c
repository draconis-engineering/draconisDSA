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

#include "trees.h"

#include <stdio.h>
#include <stdlib.h>

// ================== //
// -- BINARY TREES -- //
// ================== //

/*
 * Creates a new binary tree node with the given value, no children and no
 * parent.
 */
struct BinaryTree *btree_create(void *val) {
	struct BinaryTree *tree = malloc(sizeof(struct BinaryTree));
	if (tree == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return NULL;
	}
	tree->val = val;
	tree->parent = NULL;
	tree->left = NULL;
	tree->right = NULL;
	return tree;
}

/*
 * Recursively frees the subtree rooted at `tree`.
 */
void btree_free(struct BinaryTree *tree) {
	if (tree == NULL) {
		return;
	}
	btree_free(tree->left);
	btree_free(tree->right);
	free(tree);
}

/*
 * Returns the value stored in the node, or NULL if the node itself is NULL.
 */
void *btree_get_val(const struct BinaryTree *tree) {
	return (tree != NULL) ? tree->val : NULL;
}

/*
 * Returns the left child, or NULL.
 */
struct BinaryTree *btree_get_left(const struct BinaryTree *tree) {
	return (tree != NULL) ? tree->left : NULL;
}

/*
 * Returns the right child, or NULL.
 */
struct BinaryTree *btree_get_right(const struct BinaryTree *tree) {
	return (tree != NULL) ? tree->right : NULL;
}

/*
 * Returns the parent, or NULL for the root.
 */
struct BinaryTree *btree_get_parent(const struct BinaryTree *tree) {
	return (tree != NULL) ? tree->parent : NULL;
}

/*
 * Replaces the node's payload.
 */
void btree_set_val(struct BinaryTree *tree, void *val) {
	if (tree != NULL) {
		tree->val = val;
	}
}

/*
 * Replaces the left child, freeing any previously attached subtree and wiring
 * the new child's parent pointer.
 */
void btree_set_left(struct BinaryTree *tree, struct BinaryTree *child) {
	if (tree == NULL || tree->left == child) {
		return;
	}
	btree_free(tree->left);
	tree->left = child;
	if (child != NULL) {
		child->parent = tree;
	}
}

/*
 * Replaces the right child, freeing any previously attached subtree and wiring
 * the new child's parent pointer.
 */
void btree_set_right(struct BinaryTree *tree, struct BinaryTree *child) {
	if (tree == NULL || tree->right == child) {
		return;
	}
	btree_free(tree->right);
	tree->right = child;
	if (child != NULL) {
		child->parent = tree;
	}
}

/*
 * Creates a new node with `val` and attaches it as the left child, freeing any
 * previously attached subtree. Returns the new node, or NULL on failure.
 */
struct BinaryTree *btree_insert_left(struct BinaryTree *tree, void *val) {
	if (tree == NULL) {
		return NULL;
	}
	struct BinaryTree *child = btree_create(val);
	if (child == NULL) {
		return NULL;
	}
	btree_set_left(tree, child);
	return child;
}

/*
 * Creates a new node with `val` and attaches it as the right child, freeing
 * any previously attached subtree. Returns the new node, or NULL on failure.
 */
struct BinaryTree *btree_insert_right(struct BinaryTree *tree, void *val) {
	if (tree == NULL) {
		return NULL;
	}
	struct BinaryTree *child = btree_create(val);
	if (child == NULL) {
		return NULL;
	}
	btree_set_right(tree, child);
	return child;
}

/*
 * Returns whether the node has no children.
 */
bool btree_is_leaf(const struct BinaryTree *tree) {
	return tree != NULL && tree->left == NULL && tree->right == NULL;
}

/*
 * Returns the height of the subtree rooted at `tree`: the number of edges on
 * the longest downward path to a leaf. A single node has height 0; NULL has
 * height -1.
 */
int btree_height(const struct BinaryTree *tree) {
	if (tree == NULL) {
		return -1;
	}
	int left = btree_height(tree->left);
	int right = btree_height(tree->right);
	return (left > right ? left : right) + 1;
}

/*
 * Returns the number of nodes in the subtree rooted at `tree`.
 */
int btree_size(const struct BinaryTree *tree) {
	if (tree == NULL) {
		return 0;
	}
	return 1 + btree_size(tree->left) + btree_size(tree->right);
}

/*
 * Visits root, then left, then right.
 */
void btree_traverse_preorder(const struct BinaryTree *tree,
							 void (*visit)(void *)) {
	if (tree == NULL) {
		return;
	}
	visit(tree->val);
	btree_traverse_preorder(tree->left, visit);
	btree_traverse_preorder(tree->right, visit);
}

/*
 * Visits left, then root, then right. For a binary search tree this visits
 * nodes in sorted order.
 */
void btree_traverse_inorder(const struct BinaryTree *tree,
							void (*visit)(void *)) {
	if (tree == NULL) {
		return;
	}
	btree_traverse_inorder(tree->left, visit);
	visit(tree->val);
	btree_traverse_inorder(tree->right, visit);
}

/*
 * Visits left, then right, then root.
 */
void btree_traverse_postorder(const struct BinaryTree *tree,
							  void (*visit)(void *)) {
	if (tree == NULL) {
		return;
	}
	btree_traverse_postorder(tree->left, visit);
	btree_traverse_postorder(tree->right, visit);
	visit(tree->val);
}

// =================== //
// -- REGULAR TREES -- //
// =================== //

/*
 * Creates a new n-ary tree node with the given value and no children.
 */
struct Tree *tree_create(void *val) {
	struct Tree *tree = malloc(sizeof(struct Tree));
	if (tree == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		return NULL;
	}
	tree->val = val;
	tree->num_children = 0;
	tree->capacity = 0;
	tree->children = NULL;
	return tree;
}

/*
 * Recursively frees the subtree rooted at `tree`.
 */
void tree_free(struct Tree *tree) {
	if (tree == NULL) {
		return;
	}
	for (int i = 0; i < tree->num_children; i++) {
		tree_free(tree->children[i]);
	}
	free(tree->children);
	free(tree);
}

/*
 * Appends a new child holding `child_val` to `parent` and returns it, or NULL
 * on failure.
 */
struct Tree *tree_add_child(struct Tree *parent, void *child_val) {
	if (parent == NULL) {
		return NULL;
	}

	if (parent->num_children == parent->capacity) {
		int new_capacity = parent->capacity == 0 ? 4 : parent->capacity * 2;
		struct Tree **new_children =
			realloc(parent->children, new_capacity * sizeof(struct Tree *));
		if (new_children == NULL) {
			fprintf(stderr, "Error: realloc failed\n");
			return NULL;
		}
		parent->children = new_children;
		parent->capacity = new_capacity;
	}

	struct Tree *child = tree_create(child_val);
	if (child == NULL) {
		return NULL;
	}

	parent->children[parent->num_children++] = child;
	return child;
}

/*
 * Returns the number of children attached to the node.
 */
int tree_num_children(const struct Tree *tree) {
	return (tree != NULL) ? tree->num_children : 0;
}