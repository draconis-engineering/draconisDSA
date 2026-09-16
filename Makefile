CC = gcc
CFLAGS = -Wall -Wextra -Isrc

# Automatically find all .c files in src/ and its subdirectories
SRCS = $(shell find src -name "*.c")
OBJS = $(SRCS:.c=.o)
TARGET = main.exe

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
