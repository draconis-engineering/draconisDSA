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

#ifndef SEARCH_H
#define SEARCH_H

#include <stddef.h>

#include "shared/shared.h"

/**
 * @brief Find `target` by walking the array from front to back.
 *
 * Linear search works on any array, sorted or not, and it is the only
 * search that can find an element in an unsorted array.
 *
 * @param items The array to search. May be NULL.
 * @param count Number of elements in `items`.
 * @param target The element to look for.
 * @param cmp Comparator used to compare two elements. Must not be NULL.
 * @return The index of the first match, or -1 if there is no match.
 */
long algo_linear_search(void **items, size_t count, void *target, cmp_fn cmp);

/**
 * @brief Find `target` in a sorted array by repeatedly halving the range.
 *
 * Binary search is much faster than a linear search, but it only works on
 * an array that is already sorted with the same comparator.
 *
 * @param items The sorted array to search. May be NULL.
 * @param count Number of elements in `items`.
 * @param target The element to look for.
 * @param cmp Comparator used to compare two elements. Must not be NULL.
 * @return The index of a match, or -1 if there is no match.
 */
long algo_binary_search(void **items, size_t count, void *target, cmp_fn cmp);

#endif // SEARCH_H
