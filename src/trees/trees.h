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

/**
 * @brief A node in a binary tree.
 */
struct BinaryTree {
	/** Payload stored in the node. */
	void *val;
	/** Pointer to the parent node, or NULL if root. */
	struct BinaryTree *parent;
	/** Pointer to the left child, or NULL. */
	struct BinaryTree *left;
	/** Pointer to the right child, or NULL. */
	struct BinaryTree *right;
};

/**
 * @brief Create a new binary tree node with no children and no parent.
 * @param val Pointer to the payload to store.
 * @return A pointer to the new node, or NULL on allocation failure.
 */
struct BinaryTree *create_btree(void *val);

/**
 * @brief Recursively free the node and all descendants. NULL-safe.
 * @param tree Pointer to the root of the subtree to free.
 */
void btree_free(struct BinaryTree *tree);

/**
 * @brief Get the payload of a node. NULL-safe.
 * @param tree Pointer to the node.
 * @return The payload, or NULL if the node is NULL.
 */
void *btree_get_val(const struct BinaryTree *tree);

/**
 * @brief Get the left child of a node. NULL-safe.
 * @param tree Pointer to the node.
 * @return Pointer to the left child, or NULL.
 */
struct BinaryTree *btree_get_left(const struct BinaryTree *tree);

/**
 * @brief Get the right child of a node. NULL-safe.
 * @param tree Pointer to the node.
 * @return Pointer to the right child, or NULL.
 */
struct BinaryTree *btree_get_right(const struct BinaryTree *tree);

/**
 * @brief Get the parent of a node. NULL-safe.
 * @param tree Pointer to the node.
 * @return Pointer to the parent node, or NULL if root.
 */
struct BinaryTree *btree_get_parent(const struct BinaryTree *tree);

/**
 * @brief Replace the node's payload.
 * @param tree Pointer to the node.
 * @param val New payload.
 */
void btree_set_val(struct BinaryTree *tree, void *val);

/**
 * @brief Replace the left child, freeing any previously attached subtree and wiring the new child's parent.
 * @param tree Pointer to the node.
 * @param child New left child, or NULL to remove.
 */
void btree_set_left(struct BinaryTree *tree, struct BinaryTree *child);

/**
 * @brief Replace the right child, freeing any previously attached subtree and wiring the new child's parent.
 * @param tree Pointer to the node.
 * @param child New right child, or NULL to remove.
 */
void btree_set_right(struct BinaryTree *tree, struct BinaryTree *child);

/**
 * @brief Create a new node and attach it as left child, freeing any previous subtree.
 * @param tree Pointer to the node.
 * @param val Payload for the new node.
 * @return Pointer to the new node, or NULL on failure.
 */
struct BinaryTree *btree_insert_left(struct BinaryTree *tree, void *val);

/**
 * @brief Create a new node and attach it as right child, freeing any previous subtree.
 * @param tree Pointer to the node.
 * @param val Payload for the new node.
 * @return Pointer to the new node, or NULL on failure.
 */
struct BinaryTree *btree_insert_right(struct BinaryTree *tree, void *val);

/**
 * @brief Check if the node is a leaf (no children).
 * @param tree Pointer to the node.
 * @return True if leaf, false otherwise.
 */
bool btree_is_leaf(const struct BinaryTree *tree);

/**
 * @brief Get the height of the subtree (number of edges on longest downward path to leaf).
 * @param tree Pointer to the root of the subtree.
 * @return Height; leaf returns 0, NULL returns -1.
 */
int btree_height(const struct BinaryTree *tree);

/**
 * @brief Get the total number of nodes in the subtree.
 * @param tree Pointer to the root of the subtree.
 * @return Number of nodes, or 0 if NULL.
 */
int btree_size(const struct BinaryTree *tree);

/**
 * @brief Preorder traversal (root, left, right).
 * @param tree Pointer to the root of the subtree.
 * @param visit Callback function called for each node's payload.
 */
void btree_traverse_preorder(const struct BinaryTree *tree,
							 void (*visit)(void *));

/**
 * @brief Inorder traversal (left, root, right).
 * @param tree Pointer to the root of the subtree.
 * @param visit Callback function called for each node's payload.
 */
void btree_traverse_inorder(const struct BinaryTree *tree,
							void (*visit)(void *));

/**
 * @brief Postorder traversal (left, right, root).
 * @param tree Pointer to the root of the subtree.
 * @param visit Callback function called for each node's payload.
 */
void btree_traverse_postorder(const struct BinaryTree *tree,
							  void (*visit)(void *));

// =================== //
// -- REGULAR TREES -- //
// =================== //

/**
 * @brief An n-ary tree node. Children are stored in a growable array.
 */
struct Tree {
	/** Payload stored in the node. */
	void *val;
	/** Number of children currently attached. */
	int num_children;
	/** Current capacity of the children array. */
	int capacity;
	/** Array of pointers to child nodes. */
	struct Tree **children;
};

/**
 * @brief Create a new n-ary tree node with no children.
 * @param val Pointer to the payload to store.
 * @return A pointer to the new node, or NULL on allocation failure.
 */
struct Tree *create_tree(void *val);

/**
 * @brief Recursively free the node and all descendants. NULL-safe.
 * @param tree Pointer to the root of the subtree to free.
 */
void tree_free(struct Tree *tree);

/**
 * @brief Append a new child with the given value and return it.
 * @param parent Pointer to the parent node.
 * @param child_val Payload for the new child.
 * @return Pointer to the new child node, or NULL on failure.
 */
struct Tree *tree_add_child(struct Tree *parent, void *child_val);

/**
 * @brief Get the number of children of a node.
 * @param tree Pointer to the node.
 * @return Number of children, or 0 if NULL.
 */
int tree_num_children(const struct Tree *tree);

#endif // TREES_H
