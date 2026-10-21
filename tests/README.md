# Tests

The test suite is written from scratch, with no external dependencies. It uses
a tiny harness that lives in `tests/test.h` and `tests/test.c`.

## Running the tests

```sh
make test
```

That builds `tests/run_tests.exe` and runs it. The exit code is 0 when every
test passed and 1 otherwise, so it works as a CI gate.

To run the same tests under the address and undefined behaviour sanitizers:

```sh
make test-sanitize
```

This builds into `build-sanitize/` with `-fsanitize=address,undefined`. It
catches out-of-bounds access, use-after-free, memory leaks and undefined
behaviour. The sanitized build has to stay clean.

The library under test prints diagnostics to stdout, which would drown the
report. The harness therefore points stdout at the null device and keeps its
own copy for the report. To see the library output as well, set:

```sh
DRACONIS_TEST_VERBOSE=1 ./tests/run_tests.exe
```

## Layout

| File             | Purpose                                              |
| ---------------- | ---------------------------------------------------- |
| `tests/test.h`   | The harness API: `TEST`, `RUN_TEST` and the assertions |
| `tests/test.c`   | The harness implementation and the report            |
| `tests/tests.h`  | One `run_<module>_tests()` declaration per module    |
| `tests/test_main.c` | The runner that calls every module entry point    |
| `tests/test_<module>.c` | The test cases for one module                 |

## Writing a test case

A test case is a plain function that exercises one behaviour and reports
problems with the `ASSERT_*` macros. Declare it with `TEST()` at file scope
and register it with `RUN_TEST()`:

```c
#include "test.h"
#include "tests.h"

#include "heap/heap.h"

// Orders ints ascending, which makes the heap a min-heap.
static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

// A new heap is empty.
TEST(test_heap_create_is_empty) {
    struct Heap *heap = create_heap(cmp_int);

    ASSERT_NOT_NULL(heap);
    ASSERT_EQ_UINT(heap_size(heap), 0);
    ASSERT(heap_is_empty(heap));

    free_heap(heap);
}
```

The name of the function becomes the name in the report, so pick something
descriptive: `test_heap_pops_in_ascending_order` reads much better than
`test_heap_2`.

## The assertions

| Macro                        | Checks                                     |
| ---------------------------- | ------------------------------------------ |
| `ASSERT(cond)`               | The condition is true                      |
| `ASSERT_FALSE(cond)`         | The condition is false                     |
| `ASSERT_EQ_INT(a, b)`        | Two integers are equal                     |
| `ASSERT_EQ_LONG(a, b)`       | Two longs are equal                        |
| `ASSERT_EQ_UINT(a, b)`       | Two unsigned integers are equal            |
| `ASSERT_EQ_STR(a, b)`        | Two strings are equal (NULL-safe)          |
| `ASSERT_NULL(ptr)`           | The pointer is NULL                        |
| `ASSERT_NOT_NULL(ptr)`       | The pointer is not NULL                    |
| `ASSERT_PTR_EQ(a, b)`        | Two pointers point at the same address     |

A failing assertion does not stop the test. The remaining assertions in the
same test still run, which usually turns one run into several useful bug
reports. A test that fails is reported as `[FAIL]` with the file, the line and
both values.

## Adding a module

1. Write the test cases in `tests/test_<module>.c`.
2. Add a `void run_<module>_tests(void);` declaration to `tests/tests.h`.
3. Add a `run_<module>_tests()` function at the end of the new test file that
   calls `test_section()` and then `RUN_TEST()` for each test case.
4. Call `run_<module>_tests()` from `tests/test_main.c`.

The Makefile picks up new files in `tests/` automatically, so there is nothing
to change there.

## Guidelines

- **One behaviour per test.** A test that checks three unrelated things is
  hard to read and hard to debug.
- **Free everything.** Every `create_*` needs a matching `free_*`, otherwise
  the sanitized build reports a leak.
- **Test the edges.** Empty containers, a single element, the first and the
  last position, and invalid arguments.
- **Name the test after what it checks**, not after the function it calls.

## A gotcha worth knowing

The `head` and `tail` fields of the list containers are **node pointers**, not
payload pointers. To compare against the value you stored, reach through the
node:

```c
ASSERT_PTR_EQ(l->head->data, &my_value);   // right
ASSERT_PTR_EQ(l->head, &my_value);          // wrong
```
