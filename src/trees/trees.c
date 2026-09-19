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

struct BinaryTree *create_btree(void *root_val) {
	struct BinaryTree *tree = malloc(sizeof(struct BinaryTree));
	tree->val = root_val;
	tree->l = NULL;
	tree->r = NULL;
	return tree;
}

void free_btree(struct BinaryTree *tree) {
	if (tree == NULL)
		return;
	free_btree(tree->l);
	free_btree(tree->r);
	free(tree);
}

void *get_val_btree(struct BinaryTree *tree) { return tree->val; }
