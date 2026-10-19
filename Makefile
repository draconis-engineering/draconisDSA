CC = gcc
CFLAGS = -Wall -Wextra -std=c23 -Isrc

TARGET = main.exe

SOURCES = $(wildcard src/*.c) \
          $(wildcard src/*/*.c)

OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
