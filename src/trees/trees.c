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
#include <stdlib.h>

// ================== //
// -- BINARY TREES -- //
// ================== //

/*
 * Creates a new binary tree with the given root value.
 */
struct BinaryTree *create_btree(void *root_val) {
	struct BinaryTree *tree = malloc(sizeof(struct BinaryTree));
	tree->val = root_val;
	tree->l = NULL;
	tree->r = NULL;
	tree->parent = NULL;
	return tree;
}

/*
 * Sets the values of the tree to the given values, freeing any existing left
 * subtree. If the values are NULL, the corresponding values are not modified.
 * If a value is given, a new tree gets created with the corresponding given
 * value as a root value.
 */
void add_values(struct BinaryTree *tree, void *lv, void *rv) {
	if (tree == NULL) {
		return;
	}

	if (lv != NULL) {
		tree->l = create_btree(lv);
	}
	if (rv != NULL) {
		tree->r = create_btree(rv);
	}

	return;
}

/*
 * Sets the values of the tree to the given values, without freeing any existing
 * left or right subtree. If the values are NULL, the corresponding values are
 * not modified. Does not create new trees if the values are non-NULL.
 */
void set_values(struct BinaryTree *tree, void *lv, void *rv) {
	if (tree == NULL) {
		return;
	}
	if (lv != NULL) {
		tree->l->val = lv;
	}
	if (rv != NULL) {
		tree->r->val = rv;
	}
	return;
}

// =================== //
// -- REGULAR TREES -- //
// =================== //
