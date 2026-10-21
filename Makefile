CC = gcc
CFLAGS = -Wall -Wextra -std=c2x -Isrc -MMD -MP

# Directory that holds the compiled objects. Point it somewhere else to build
# a second configuration without disturbing the first one, for example:
#
#     make test-sanitize
#
BUILD ?= .

TARGET = $(BUILD)/main.exe
TEST_TARGET = $(BUILD)/tests/run_tests.exe

# Every translation unit in the library. The demo entry point is excluded so
# that the test binary can link the library without a second main().
LIB_SOURCES = $(filter-out src/main.c, $(wildcard src/*.c) $(wildcard src/*/*.c))
MAIN_SOURCE = src/main.c
TEST_SOURCES = $(wildcard tests/*.c)

# Object file for every source file, placed under $(BUILD).
LIB_OBJECTS = $(patsubst %.c,$(BUILD)/%.o,$(LIB_SOURCES))
MAIN_OBJECT = $(patsubst %.c,$(BUILD)/%.o,$(MAIN_SOURCE))
TEST_OBJECTS = $(patsubst %.c,$(BUILD)/%.o,$(TEST_SOURCES))

OBJECTS = $(LIB_OBJECTS) $(MAIN_OBJECT) $(TEST_OBJECTS)
DEPS = $(OBJECTS:.o=.d)

.PHONY: all clean test test-sanitize

all: $(TARGET)

$(TARGET): $(MAIN_OBJECT) $(LIB_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build and run the test suite.
test: $(TEST_TARGET)
	@./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(LIB_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Run the test suite with the address and undefined behaviour sanitizers.
# This catches out-of-bounds access, use-after-free and integer overflow.
test-sanitize:
	@$(MAKE) --no-print-directory test BUILD=build-sanitize \
		CFLAGS="$(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer -g"

# Rebuild objects when the headers they include change.
-include $(DEPS)

clean:
	$(RM) $(OBJECTS) $(DEPS) $(TARGET) $(TEST_TARGET)
	$(RM) -r build-sanitize
