/*
 * Shared Library - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#ifndef SHARED_H
#define SHARED_H

#include <stddef.h>

/**
 * @brief Comparator used by the sorting, searching and heap modules.
 *
 * The comparator receives two elements and decides their relative order:
 *
 * - a negative value means `a` comes **before** `b`
 * - zero means the two elements are equivalent
 * - a positive value means `a` comes **after** `b`
 *
 * The signature matches the one used by the C standard library's `qsort()`,
 * so an existing comparator can be handed to every algorithm in this library
 * without any change.
 *
 * @param a The first element to compare.
 * @param b The second element to compare.
 * @return A negative value, zero, or a positive value as described above.
 */
typedef int (*cmp_fn)(const void *a, const void *b);

#endif // SHARED_H
