# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Added

- Dynamic array (`src/array`) with a `void*` API: `create_array`, `arr_size`, `arr_is_empty`, `arr_push`, `arr_insert`, `arr_get`, `arr_set`, `arr_remove`, `arr_pop`, `print_array`, `free_array`. Grows by doubling its capacity for amortized O(1) appends.
- Array demo in `src/main.c`.
- Dynamic array explanation (`explainations/Array.en.md`).
