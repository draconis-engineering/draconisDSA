/*
 * Test Harness - DRACONIS ENGINEERING DATA STRUCTURES & ALGORITHMS
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

#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/**
 * @brief Signature of a test case.
 *
 * A test case is a plain function that exercises one behaviour and reports
 * problems through the ASSERT_* macros. It should never abort the process:
 * using ASSERT_* lets the remaining assertions in the same test still run,
 * which usually turns one run into several useful bug reports.
 */
typedef void (*test_fn)(void);

/** @brief Number of assertions evaluated across the whole suite. */
extern int test_assertions;

/**
 * @brief Print the banner and prepare the report stream.
 *
 * The library under test writes diagnostic output to stdout. The harness
 * moves that output out of the way so the report stays readable; set the
 * environment variable DRACONIS_TEST_VERBOSE to keep it.
 *
 * @param suite_name Name of the suite, shown in the banner.
 */
void test_begin(const char *suite_name);

/**
 * @brief Print the summary of the run.
 * @return 0 if every test passed, 1 if at least one failed.
 */
int test_end(void);

/**
 * @brief Print a section heading. Use one per module.
 * @param name Name of the section.
 */
void test_section(const char *name);

/**
 * @brief Run a single test case and report whether it passed.
 * @param name Name of the test case.
 * @param fn The test case to run.
 */
void test_run(const char *name, test_fn fn);

/**
 * @brief Record an assertion failure. Called by the ASSERT_* macros.
 * @param file Source file of the failing assertion.
 * @param line Line number of the failing assertion.
 * @param fmt printf-style message describing the failure.
 */
void test_fail(const char *file, int line, const char *fmt, ...);

/**
 * @brief Declare a test case.
 *
 * Use it at file scope:
 *
 *     TEST(test_array_push_grows) {
 *         ...
 *     }
 *
 * Declaring a test does not run it. Add it to the module's runner with
 * RUN_TEST() so the suite picks it up.
 */
#define TEST(name) static void name(void)

/**
 * @brief Run a test case declared with TEST().
 * @param fn The test case to run.
 */
#define RUN_TEST(fn) test_run(#fn, fn)

/**
 * @brief Assert that a condition holds.
 * @param cond The condition that must be true.
 */
#define ASSERT(cond)                                                       \
	do {                                                                   \
		test_assertions++;                                                 \
		if (!(cond)) {                                                     \
			test_fail(__FILE__, __LINE__, "expected %s", #cond);           \
		}                                                                  \
	} while (0)

/**
 * @brief Assert that a condition does not hold.
 * @param cond The condition that must be false.
 */
#define ASSERT_FALSE(cond)                                                 \
	do {                                                                   \
		test_assertions++;                                                 \
		if ((cond)) {                                                      \
			test_fail(__FILE__, __LINE__, "expected NOT %s", #cond);       \
		}                                                                  \
	} while (0)

/**
 * @brief Assert that two integers are equal.
 * @param actual The value produced by the code under test.
 * @param expected The value it should have produced.
 */
#define ASSERT_EQ_INT(actual, expected)                                    \
	do {                                                                   \
		long long assert_actual = (long long)(actual);                     \
		long long assert_expected = (long long)(expected);                 \
		test_assertions++;                                                 \
		if (assert_actual != assert_expected) {                            \
			test_fail(__FILE__, __LINE__,                                  \
					  "%s: expected %lld, got %lld", #actual,              \
					  assert_expected, assert_actual);                      \
		}                                                                  \
	} while (0)

/**
 * @brief Assert that two longs are equal.
 * @param actual The value produced by the code under test.
 * @param expected The value it should have produced.
 */
#define ASSERT_EQ_LONG(actual, expected)                                    \
	do {                                                                   \
		long assert_actual = (long)(actual);                               \
		long assert_expected = (long)(expected);                           \
		test_assertions++;                                                 \
		if (assert_actual != assert_expected) {                            \
			test_fail(__FILE__, __LINE__,                                  \
					  "%s: expected %ld, got %ld", #actual,                \
					  assert_expected, assert_actual);                      \
		}                                                                  \
	} while (0)

/**
 * @brief Assert that two unsigned integers are equal.
 * @param actual The value produced by the code under test.
 * @param expected The value it should have produced.
 */
#define ASSERT_EQ_UINT(actual, expected)                                   \
	do {                                                                   \
		unsigned long long assert_actual = (unsigned long long)(actual);   \
		unsigned long long assert_expected = (unsigned long long)(expected);\
		test_assertions++;                                                 \
		if (assert_actual != assert_expected) {                            \
			test_fail(__FILE__, __LINE__,                                  \
					  "%s: expected %llu, got %llu", #actual,              \
					  assert_expected, assert_actual);                      \
		}                                                                  \
	} while (0)

/**
 * @brief Assert that two strings are equal (NULL-safe).
 * @param actual The string produced by the code under test.
 * @param expected The string it should have produced.
 */
#define ASSERT_EQ_STR(actual, expected)                                    \
	do {                                                                   \
		const char *assert_actual = (actual);                              \
		const char *assert_expected = (expected);                          \
		int assert_differs = (assert_actual == NULL || assert_expected == NULL) \
								 ? (assert_actual != assert_expected)     \
								 : (strcmp(assert_actual, assert_expected) != 0); \
		test_assertions++;                                                 \
		if (assert_differs) {                                              \
			test_fail(__FILE__, __LINE__,                                  \
					  "%s: expected \"%s\", got \"%s\"", #actual,           \
					  assert_expected ? assert_expected : "(null)",         \
					  assert_actual ? assert_actual : "(null)");             \
		}                                                                  \
	} while (0)

/**
 * @brief Assert that a pointer is NULL.
 * @param ptr The pointer that should be NULL.
 */
#define ASSERT_NULL(ptr)                                                   \
	do {                                                                   \
		const void *assert_ptr = (const void *)(ptr);                      \
		test_assertions++;                                                 \
		if (assert_ptr != NULL) {                                          \
			test_fail(__FILE__, __LINE__, "%s: expected NULL, got %p",     \
					  #ptr, (void *)assert_ptr);                           \
		}                                                                  \
	} while (0)

/**
 * @brief Assert that a pointer is not NULL.
 * @param ptr The pointer that should not be NULL.
 */
#define ASSERT_NOT_NULL(ptr)                                               \
	do {                                                                   \
		const void *assert_ptr = (const void *)(ptr);                      \
		test_assertions++;                                                 \
		if (assert_ptr == NULL) {                                          \
			test_fail(__FILE__, __LINE__, "%s: expected a non-NULL pointer",\
					  #ptr);                                               \
		}                                                                  \
	} while (0)

/**
 * @brief Assert that two pointers refer to the same address.
 * @param actual The pointer produced by the code under test.
 * @param expected The pointer it should be identical to.
 */
#define ASSERT_PTR_EQ(actual, expected)                                    \
	do {                                                                   \
		const void *assert_actual = (const void *)(actual);                \
		const void *assert_expected = (const void *)(expected);            \
		test_assertions++;                                                 \
		if (assert_actual != assert_expected) {                            \
			test_fail(__FILE__, __LINE__,                                  \
					  "%s: expected pointer %p, got %p", #actual,           \
					  (void *)assert_expected, (void *)assert_actual);       \
		}                                                                  \
	} while (0)

#endif // TEST_H
