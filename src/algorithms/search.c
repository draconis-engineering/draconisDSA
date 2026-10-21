/*
 * Searching Algorithms - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#include "search.h"

/*
 * Linear search: walk the array and stop at the first match.
 */
long algo_linear_search(void **items, size_t count, void *target, cmp_fn cmp) {
	if (items == NULL || cmp == NULL) {
		return -1;
	}

	for (size_t i = 0; i < count; i++) {
		if (cmp(items[i], target) == 0) {
			return (long)i;
		}
	}

	return -1;
}

/*
 * Binary search: compare the target with the middle element of the range and
 * throw away the half that cannot hold it.
 */
long algo_binary_search(void **items, size_t count, void *target, cmp_fn cmp) {
	if (items == NULL || cmp == NULL || count == 0) {
		return -1;
	}

	size_t low = 0;
	size_t high = count - 1;

	while (low <= high) {
		size_t middle = low + (high - low) / 2;
		int order = cmp(items[middle], target);

		if (order == 0) {
			return (long)middle;
		}

		if (order < 0) {
			// The target sorts after the middle element.
			low = middle + 1;
		} else {
			// The target sorts before the middle element.
			if (middle == 0) {
				break;
			}
			high = middle - 1;
		}
	}

	return -1;
}
