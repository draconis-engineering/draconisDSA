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

#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

// DJB2 Hashing Algorithm (Simple and highly effective for strings)
unsigned int hash(const char* key) {
    unsigned long int value = 5381;
    int i = 0;
    while (key[i] != '\0') {
        value = ((value << 5) + value) + key[i]; // value * 33 + key[i]
        i++;
    }
    return value % BUCKETS;
}

// Initialize the map
HashMap* create_map() {
    HashMap* map = malloc(sizeof(HashMap));
    for (int i = 0; i < BUCKETS; i++) {
        map->buckets[i] = NULL;
    }
    return map;
}

// Insert or Update a key-value pair
void map_insert(HashMap* map, const char* key, int value) {
    unsigned int index = hash(key);
    Node* current = map->buckets[index];

    // Check if key already exists, if so update it
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    // Key doesn't exist, create a new node and prepend it (O(1) insertion)
    Node* new_node = malloc(sizeof(Node));
    new_node->key = strdup(key); // Duplicate string to manage memory safely
    new_node->value = value;
    new_node->next = map->buckets[index];
    map->buckets[index] = new_node;
}

// Retrieve a value
int map_get(HashMap* map, const char* key, int* found) {
    unsigned int index = hash(key);
    Node* current = map->buckets[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            *found = 1;
            return current->value;
        }
        current = current->next;
    }
    *found = 0;
    return -1; // Default error return
}
