/*
 * Tree Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "test.h"
#include "tests.h"

#include "trees/trees.h"

// Buffer that collects the values seen by a traversal callback.
static const char *visited[32];
static int visited_count;

// Traversal callback that records every visited value.
static void collect(void *val) {
	if (visited_count < 32) {
		visited[visited_count++] = (const char *)val;
	}
}

// Clears the collection buffer before a traversal.
static void reset_visited(void) { visited_count = 0; }

// Builds the tree used by most of the tests:
//
//         root
//        /    \
//      left    right
//     /   \
//   ll    lr
static struct BinaryTree *build_tree(void) {
	struct BinaryTree *root = create_btree((void *)"root");
	struct BinaryTree *left = btree_insert_left(root, (void *)"left");
	btree_insert_right(root, (void *)"right");
	btree_insert_left(left, (void *)"ll");
	btree_insert_right(left, (void *)"lr");
	return root;
}

// A new binary tree node has a value but no relatives.
TEST(test_btree_create) {
	struct BinaryTree *root = create_btree((void *)"root");

	ASSERT_NOT_NULL(root);
	ASSERT_EQ_STR(btree_get_val(root), "root");
	ASSERT_NULL(btree_get_left(root));
	ASSERT_NULL(btree_get_right(root));
	ASSERT_NULL(btree_get_parent(root));
	ASSERT(btree_is_leaf(root));

	btree_free(root);
}

// Accessors are NULL-safe.
TEST(test_btree_accessors_are_null_safe) {
	ASSERT_NULL(btree_get_val(NULL));
	ASSERT_NULL(btree_get_left(NULL));
	ASSERT_NULL(btree_get_right(NULL));
	ASSERT_NULL(btree_get_parent(NULL));
	ASSERT_FALSE(btree_is_leaf(NULL));
	ASSERT_EQ_INT(btree_height(NULL), -1);
	ASSERT_EQ_INT(btree_size(NULL), 0);
}

// Inserting children wires up the parent pointers.
TEST(test_btree_insert_children) {
	struct BinaryTree *root = create_btree((void *)"root");
	struct BinaryTree *left = btree_insert_left(root, (void *)"left");
	struct BinaryTree *right = btree_insert_right(root, (void *)"right");

	ASSERT_NOT_NULL(left);
	ASSERT_NOT_NULL(right);
	ASSERT_PTR_EQ(btree_get_left(root), left);
	ASSERT_PTR_EQ(btree_get_right(root), right);
	ASSERT_PTR_EQ(btree_get_parent(left), root);
	ASSERT_PTR_EQ(btree_get_parent(right), root);
	ASSERT_FALSE(btree_is_leaf(root));
	ASSERT(btree_is_leaf(left));

	btree_free(root);
}

// The height counts the edges on the longest path down to a leaf.
TEST(test_btree_height_and_size) {
	struct BinaryTree *root = build_tree();

	ASSERT_EQ_INT(btree_height(root), 2);
	ASSERT_EQ_INT(btree_size(root), 5);

	btree_free(root);
}

// The three traversals visit the nodes in their documented orders.
TEST(test_btree_traversals) {
	struct BinaryTree *root = build_tree();

	reset_visited();
	btree_traverse_preorder(root, collect);
	ASSERT_EQ_INT(visited_count, 5);
	ASSERT_EQ_STR(visited[0], "root");
	ASSERT_EQ_STR(visited[1], "left");
	ASSERT_EQ_STR(visited[2], "ll");
	ASSERT_EQ_STR(visited[3], "lr");
	ASSERT_EQ_STR(visited[4], "right");

	reset_visited();
	btree_traverse_inorder(root, collect);
	ASSERT_EQ_INT(visited_count, 5);
	ASSERT_EQ_STR(visited[0], "ll");
	ASSERT_EQ_STR(visited[1], "left");
	ASSERT_EQ_STR(visited[2], "lr");
	ASSERT_EQ_STR(visited[3], "root");
	ASSERT_EQ_STR(visited[4], "right");

	reset_visited();
	btree_traverse_postorder(root, collect);
	ASSERT_EQ_INT(visited_count, 5);
	ASSERT_EQ_STR(visited[0], "ll");
	ASSERT_EQ_STR(visited[1], "lr");
	ASSERT_EQ_STR(visited[2], "left");
	ASSERT_EQ_STR(visited[3], "right");
	ASSERT_EQ_STR(visited[4], "root");

	btree_free(root);
}

// Replacing the payload of a node does not change the shape of the tree.
TEST(test_btree_set_val) {
	struct BinaryTree *root = create_btree((void *)"root");

	btree_set_val(root, (void *)"new");
	ASSERT_EQ_STR(btree_get_val(root), "new");

	btree_set_val(NULL, (void *)"ignored");
	btree_free(root);
}

// Replacing a child detaches the old subtree and adopts the new one.
TEST(test_btree_set_child_replaces_subtree) {
	struct BinaryTree *root = create_btree((void *)"root");
	btree_insert_left(root, (void *)"old");
	struct BinaryTree *fresh = create_btree((void *)"new");

	btree_set_left(root, fresh);

	ASSERT_PTR_EQ(btree_get_left(root), fresh);
	ASSERT_PTR_EQ(btree_get_parent(fresh), root);
	ASSERT_EQ_INT(btree_size(root), 2);

	btree_free(root);
}

// The same node cannot be a child of two parents at once.
TEST(test_btree_set_child_detaches_from_other_slot) {
	struct BinaryTree *root = create_btree((void *)"root");
	struct BinaryTree *child = create_btree((void *)"child");

	btree_set_left(root, child);
	btree_set_right(root, child);

	// The child was moved from the left slot to the right slot.
	ASSERT_NULL(btree_get_left(root));
	ASSERT_PTR_EQ(btree_get_right(root), child);

	btree_free(root);
}

// An n-ary tree grows its list of children on demand.
TEST(test_tree_add_child) {
	struct Tree *root = create_tree((void *)"root");

	ASSERT_NOT_NULL(root);
	ASSERT_EQ_INT(tree_num_children(root), 0);

	struct Tree *a = tree_add_child(root, (void *)"a");
	struct Tree *b = tree_add_child(root, (void *)"b");
	ASSERT_NOT_NULL(a);
	ASSERT_NOT_NULL(b);
	ASSERT_EQ_INT(tree_num_children(root), 2);
	ASSERT_EQ_INT(tree_num_children(a), 0);

	tree_add_child(b, (void *)"b1");
	tree_add_child(b, (void *)"b2");
	tree_add_child(b, (void *)"b3");
	ASSERT_EQ_INT(tree_num_children(b), 3);

	tree_free(root);
}

// The n-ary helpers are NULL-safe.
TEST(test_tree_handles_null) {
	ASSERT_EQ_INT(tree_num_children(NULL), 0);
	ASSERT_NULL(tree_add_child(NULL, (void *)"child"));
}

// Freeing is safe with a NULL pointer.
TEST(test_tree_free_is_null_safe) {
	btree_free(NULL);
	tree_free(NULL);
}

// Runs every tree test case.
void run_trees_tests(void) {
	test_section("trees");

	RUN_TEST(test_btree_create);
	RUN_TEST(test_btree_accessors_are_null_safe);
	RUN_TEST(test_btree_insert_children);
	RUN_TEST(test_btree_height_and_size);
	RUN_TEST(test_btree_traversals);
	RUN_TEST(test_btree_set_val);
	RUN_TEST(test_btree_set_child_replaces_subtree);
	RUN_TEST(test_btree_set_child_detaches_from_other_slot);
	RUN_TEST(test_tree_add_child);
	RUN_TEST(test_tree_handles_null);
	RUN_TEST(test_tree_free_is_null_safe);
}
