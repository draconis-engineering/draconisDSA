/*
* HashMap Library - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
* Copyright (C) 2026 Simon Stordal Amundgård
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see http://www.gnu.org/licenses.
*/

#ifndef HASHMAP_H
#define HASHMAP_H

#define BUCKETS 16

// A node in the linked list chain
typedef struct Node {
    char* key;
    int value;
    struct Node* next;
} Node;

// The main hashmap structure
typedef struct {
    Node* buckets[BUCKETS];
} HashMap;

unsigned int hash(const char* key);
HashMap* create_map();
void map_insert(HashMap* map, const char* key, int value);
int map_get(HashMap* map, const char* key, int* found);
void map_remove(HashMap* map, const char* key);

#endif // HASHMAP_H
