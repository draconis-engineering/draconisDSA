/*
 * Sort Property Tests - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

#include "test.h"
#include "tests.h"

#include "algorithms/sort.h"
#include "shared/shared.h"

#include <stdint.h>
#include <stdlib.h>

/*
 * Property tests sort random inputs and compare the result against the
 * C standard library's `qsort()`. "The sort worked" is expressed as "the
 * output agrees with the oracle", which holds for every permutation and
 * every number of duplicate keys.
 *
 * The generator is deterministic, so every run tests the same inputs. Set
 * DRACONIS_FUZZ_SEED to try one different sequence.
 */

/** Largest array the property tests sort. */
#define MAX_N 200

/** Random rounds per property. */
#define ROUNDS 40

// The signature every sorting algorithm in this library shares.
typedef bool (*sort_property_fn)(void **, size_t, cmp_fn);

/** The three stable sorts, checked for stability in a table-driven test. */
static const sort_property_fn stable_sorts[] = {
	algo_bubble_sort,
	algo_insertion_sort,
	algo_merge_sort,
};

static const int stable_sort_count = 3;

/** Starting state of the generator, advanced with splitmix64. */
static uint64_t rng_state = 0;

// Seeds the generator.
static void rng_seed(uint64_t seed) { rng_state = seed; }

// Returns the next random 64-bit number (splitmix64).
static uint64_t rng_next(void) {
	uint64_t z = (rng_state += 0x9E3779B97F4A7C15ULL);
	z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
	z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
	return z ^ (z >> 31);
}

// Returns a random number below `bound`. Requires bound > 0.
static uint32_t rng_below(uint32_t bound) { return (uint32_t)(rng_next() % bound); }

// Returns the seeds to run, honouring DRACONIS_FUZZ_SEED once.
static void property_seeds(uint64_t *seeds, int *seed_count) {
	const char *override = getenv("DRACONIS_FUZZ_SEED");
	if (override != NULL && override[0] != '\0') {
		seeds[0] = strtoull(override, NULL, 0);
		*seed_count = 1;
		return;
	}

	static const uint64_t defaults[] = {0x5027189, 0xC0FFEE, 0xB105F00D};
	for (int i = 0; i < 3; i++) {
		seeds[i] = defaults[i];
	}
	*seed_count = 3;
}

// Orders plain ints ascending.
static int cmp_int(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x > y) - (x < y);
}

// An item with a key and a sequence number, used to prove that a sort is
// stable: equal keys must keep their original relative order.
struct StableItem {
	int key;
	int seq;
};

// Comparator that only looks at the key, so equal keys are "equivalent".
static int cmp_key_only(const void *a, const void *b) {
	int x = ((const struct StableItem *)a)->key;
	int y = ((const struct StableItem *)b)->key;
	return (x > y) - (x < y);
}

// The input patterns the property tests draw from.
enum Pattern {
	PATTERN_RANDOM,       // anything goes
	PATTERN_FEW_UNIQUES,  // heavy duplicates, e.g. every value five times
	PATTERN_SORTED,       // already in order
	PATTERN_REVERSED,     // worst case for the simple sorts
	PATTERN_ALL_EQUAL,    // stress for partitioning on duplicate keys
	PATTERN_COUNT
};

// Fills `out` with `count` values shaped by `pattern`.
static void fill_pattern(int *out, size_t count, int pattern) {
	switch (pattern) {
	case PATTERN_FEW_UNIQUES:
		for (size_t i = 0; i < count; i++) {
			out[i] = (int)rng_below(8);
		}
		break;
	case PATTERN_SORTED:
		for (size_t i = 0; i < count; i++) {
			out[i] = (int)i;
		}
		break;
	case PATTERN_REVERSED:
		for (size_t i = 0; i < count; i++) {
			out[i] = (int)(count - i);
		}
		break;
	case PATTERN_ALL_EQUAL:
		for (size_t i = 0; i < count; i++) {
			out[i] = 42;
		}
		break;
	default:
		for (size_t i = 0; i < count; i++) {
			out[i] = (int)rng_below(1000);
		}
		break;
	}
}

// Sorts a copy of `values` with `sort` and checks that the result agrees
// element by element with `qsort()` on the same input.
static void check_matches_qsort(sort_property_fn sort, const int *values,
								size_t count) {
	int input[MAX_N];
	int expected[MAX_N];
	void *items[MAX_N];

	for (size_t i = 0; i < count; i++) {
		input[i] = values[i];
		expected[i] = values[i];
		items[i] = &input[i];
	}

	qsort(expected, count, sizeof(int), cmp_int);
	ASSERT(sort(items, count, cmp_int));

	for (size_t i = 0; i < count; i++) {
		ASSERT_EQ_INT(*(int *)items[i], expected[i]);
	}
}

// Random arrays of every shape, sorted by every algorithm, must produce
// exactly what the standard library's `qsort()` produces.
TEST(test_sort_property_matches_qsort) {
	uint64_t seeds[8];
	int seed_count = 0;
	property_seeds(seeds, &seed_count);

	const sort_property_fn sorts[] = {
		algo_bubble_sort,
		algo_selection_sort,
		algo_insertion_sort,
		algo_quick_sort,
		algo_heap_sort,
		algo_merge_sort,
	};

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		for (int round = 0; round < ROUNDS; round++) {
			size_t count = rng_below(MAX_N + 1);
			int pattern = (int)rng_below(PATTERN_COUNT);

			int values[MAX_N];
			fill_pattern(values, count, pattern);

			for (int i = 0; i < 6; i++) {
				check_matches_qsort(sorts[i], values, count);
			}
		}
	}
}

// Counting sort works on plain integers, so its oracle is `qsort()` on the
// same array of ints.
TEST(test_sort_property_counting_matches_qsort) {
	uint64_t seeds[8];
	int seed_count = 0;
	property_seeds(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		for (int round = 0; round < ROUNDS; round++) {
			size_t count = rng_below(MAX_N + 1);

			int input[MAX_N];
			int expected[MAX_N];

			for (size_t i = 0; i < count; i++) {
				input[i] = (int)rng_below(100);
				expected[i] = input[i];
			}

			qsort(expected, count, sizeof(int), cmp_int);
			algo_counting_sort_int(input, count);

			for (size_t i = 0; i < count; i++) {
				ASSERT_EQ_INT(input[i], expected[i]);
			}
		}
	}
}

// The stable sorts must keep equal keys in their original relative order.
// Selection, quick and heap sort are allowed to reorder equal keys, so they
// are not in this list.
TEST(test_sort_property_stable_sorts_keep_order) {
	uint64_t seeds[8];
	int seed_count = 0;
	property_seeds(seeds, &seed_count);

	for (int s = 0; s < seed_count; s++) {
		rng_seed(seeds[s]);

		for (int round = 0; round < ROUNDS; round++) {
			size_t count = rng_below(MAX_N + 1);

			for (int which = 0; which < stable_sort_count; which++) {
				struct StableItem input[MAX_N];
				void *items[MAX_N];

				// Few distinct keys, so equal keys are the rule, not the
				// exception. The sequence number is the original index.
				for (size_t i = 0; i < count; i++) {
					input[i].key = (int)rng_below(5);
					input[i].seq = (int)i;
					items[i] = &input[i];
				}

				ASSERT(stable_sorts[which](items, count, cmp_key_only));

				for (size_t i = 0; i + 1 < count; i++) {
					const struct StableItem *a = items[i];
					const struct StableItem *b = items[i + 1];

					// Keys must be in order...
					ASSERT(a->key <= b->key);
					// ...and among equal keys, the sequence numbers must
					// still grow.
					if (a->key == b->key) {
						ASSERT(a->seq < b->seq);
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

void run_sort_property_tests(void) {
	test_section("Sorting / property-based");

	RUN_TEST(test_sort_property_matches_qsort);
	RUN_TEST(test_sort_property_counting_matches_qsort);
	RUN_TEST(test_sort_property_stable_sorts_keep_order);
}
