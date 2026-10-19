CC = gcc
CFLAGS = -Wall -Wextra -std=c23 -Isrc -MMD -MP

TARGET = main.exe

SOURCES = $(wildcard src/*.c) \
          $(wildcard src/*/*.c)

OBJECTS = $(SOURCES:.c=.o)
DEPS = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rebuild objects when the headers they include change.
-include $(DEPS)

clean:
	$(RM) $(OBJECTS) $(DEPS) $(TARGET)
