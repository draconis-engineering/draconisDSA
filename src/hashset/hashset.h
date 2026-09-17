#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16

struct Node {
	char *value;
	struct Node *next;
};

struct HashSet {
	struct Node **buckets;
	size_t capacity;
	size_t size;
};

struct HashSet *create_hashset();
unsigned int hashset_hash(struct HashSet *set, const char *value);
bool hashset_contains(struct HashSet *set, const char *value);
bool hashset_add(struct HashSet *set, const char *value);
bool hashset_remove(struct HashSet *set, const char *value);
void hashset_destroy(struct HashSet *set);
