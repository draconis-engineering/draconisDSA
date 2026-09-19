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

// Binary tree
/* ----------------------------------------------------------------- */

struct BinaryTree {
	void *val;
	struct BinaryTree *l;
	struct BinaryTree *r;
};

struct BinaryTree *create_btree(void *root_val);
void free_btree(struct BinaryTree *tree);

// Regular Tree
/* ----------------------------------------------------------------- */

struct Tree {
	void *val;
	int num_children;
	struct Tree *children[];
};

struct Tree *create_tree(void *root_val);

void free_tree(struct Tree *tree);
