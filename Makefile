CC = gcc
CFLAGS = -Wall -Wextra -Isrc

# Automatically find all .c files in src/ and its subdirectories
SRCS = $(shell find src -name "*.c")
OBJS = $(SRCS:.c=.o)
TARGET = main.exe

# Build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
