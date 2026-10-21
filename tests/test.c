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

/*
 * The harness uses a few POSIX calls (dup, dup2, fdopen) to keep the
 * library's diagnostic output out of the report. Strict -std=c23 hides
 * them, so ask for them explicitly.
 */
#define _POSIX_C_SOURCE 200809L

#include "test.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#define DRACONIS_HAVE_POSIX 1
#endif

/** Number of assertions evaluated across the whole suite. */
int test_assertions = 0;

/** Where the report is written. */
static FILE *report = NULL;

/** Number of test cases executed. */
static int tests_run = 0;

/** Number of test cases that failed. */
static int tests_failed = 0;

/** Number of failures recorded inside the test case that is running. */
static int current_failures = 0;

/**
 * Prepares the report stream.
 *
 * The library under test prints diagnostics to stdout. To keep the report
 * readable, stdout is pointed at the null device while the report keeps
 * writing to the original terminal. Set DRACONIS_TEST_VERBOSE to keep the
 * library output visible.
 */
static void open_report_stream(void) {
	report = stdout;

#ifdef DRACONIS_HAVE_POSIX
	if (getenv("DRACONIS_TEST_VERBOSE") != NULL) {
		return;
	}

	FILE *null_stream = fopen("/dev/null", "w");
	if (null_stream == NULL) {
		return;
	}

	int saved_fd = dup(fileno(stdout));
	if (saved_fd == -1) {
		fclose(null_stream);
		return;
	}

	FILE *saved = fdopen(saved_fd, "w");
	if (saved == NULL) {
		close(saved_fd);
		fclose(null_stream);
		return;
	}

	// Point stdout at the null device, but keep writing the report to the
	// copy of the original terminal held by `saved`.
	if (dup2(fileno(null_stream), fileno(stdout)) == -1) {
		fclose(saved);
		fclose(null_stream);
		return;
	}

	fclose(null_stream);
	report = saved;
#endif
}

/**
 * Prints the suite banner.
 */
void test_begin(const char *suite_name) {
	open_report_stream();

	fprintf(report, "\n");
	fprintf(report, "DraconisDSA test suite :: %s\n", suite_name);
	fprintf(report, "================================================\n\n");
}

/**
 * Prints a section heading.
 */
void test_section(const char *name) {
	fprintf(report, "%s\n", name);
}

/**
 * Runs a test case and reports whether it passed.
 */
void test_run(const char *name, test_fn fn) {
	tests_run++;
	current_failures = 0;

	fn();

	if (current_failures == 0) {
		fprintf(report, "  [PASS] %s\n", name);
	} else {
		tests_failed++;
		fprintf(report, "  [FAIL] %s\n", name);
	}
}

/**
 * Records an assertion failure and prints it below the failing test.
 */
void test_fail(const char *file, int line, const char *fmt, ...) {
	current_failures++;

	fprintf(report, "         %s:%d: ", file, line);

	va_list args;
	va_start(args, fmt);
	vfprintf(report, fmt, args);
	va_end(args);

	fprintf(report, "\n");
}

/**
 * Prints the summary of the run and returns the exit code.
 */
int test_end(void) {
	fprintf(report, "\n");
	fprintf(report, "------------------------------------------------\n");
	fprintf(report, "Tests: %d, passed: %d, failed: %d, assertions: %d\n",
			tests_run, tests_run - tests_failed, tests_failed, test_assertions);
	fprintf(report, "------------------------------------------------\n");

	if (tests_failed > 0) {
		fprintf(report, "RESULT: FAILURE\n\n");
		return 1;
	}

	fprintf(report, "RESULT: SUCCESS\n\n");
	return 0;
}
