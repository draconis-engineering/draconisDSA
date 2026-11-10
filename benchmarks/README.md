# Benchmarks

The benchmark suite measures the cost of every operation the library offers
and shows how each cost reacts when the workload doubles. It lives in
`benchmarks/bench.c` and has no external dependencies.

## Running the benchmarks

```sh
make bench
```

That builds `benchmarks/bench.exe` and runs it. A full run takes roughly
five to ten seconds.

## How to read the report

Every row runs a batch of `n` operations, repeats the batch until at least
60 ms of work has been done, and keeps the fastest batch. It then repeats
the whole measurement with `2n` operations and prints:

| Column      | Meaning                                                   |
| ----------- | --------------------------------------------------------- |
| `n`         | Batch size of the first measurement                        |
| `ns/op`     | Nanoseconds per operation at `n`                           |
| `ns/op @2n` | Nanoseconds per operation at `2n`                          |
| `growth`    | Per-operation cost of `2n` divided by the cost at `n`      |

The growth column is what separates the complexity classes:

| Growth       | Meaning                                              |
| ------------ | ---------------------------------------------------- |
| ~1.0         | O(1) amortized (constant per operation)              |
| ~1.0 - 1.2   | O(log n) (heap push/pop, binary search, n log n sorts) |
| ~2.0         | O(n) per operation (middle inserts, linear scans)      |
| ~2.0 (elem)  | Quadratic total time (the O(n^2) sorts, per element)  |

A few rows are worth calling out:

- **`sll insert at back` vs `sll remove last`** - appending is O(1) because
  the list keeps a tail pointer, but removing the last node still has to
  walk from the head, so its cost doubles.
- **`hashmap` / `hashset` rows** - both containers keep a fixed number of
  buckets (no rehashing), so their chains grow linearly with the number of
  elements and every operation slowly degrades towards O(n). The growth
  column shows this honestly.
- **`graph has edge (miss)`** - the graph stores edges in a flat array and
  scans it, so a miss costs O(E).
- **`counting sort`** - linear time, but the per-element cost still creeps
  up as the array outgrows the CPU cache.

The numbers are wall-clock times from an unoptimized build, so absolute
values differ between machines; the growth factors are the stable part. To
measure an optimized build without disturbing the normal objects:

```sh
make bench BUILD=build-o2 CFLAGS="-O2 -Wall -Wextra -std=c2x -Isrc"
```

The expected costs per operation are also written down, with the reasoning
behind each of them, in [docs/COMPLEXITY.md](../docs/COMPLEXITY.md).
