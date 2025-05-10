# The compiler: gcc for C program
CC = gcc

# Compiler flags:
#  -Wall      - enables most common warnings
#  -Wextra    - enables extra warnings
#  -Wpedantic - ensures strict compliance with the C standard
#  -Werror    - treats all warnings as errors
#  -g         - includes debug information for gdb
CFLAGS  = -Wall -Wextra -Wpedantic -Werror -g

SOURCE = src/*.c

# The build target
TARGET = build/compressor

all: $(TARGET)
	@echo "\n Build completed successfully! Run ./$(TARGET) to execute your program!"

$(TARGET): $(SOURCE)
	$(CC) -o $(TARGET) $(SOURCE) $(CFLAGS)

clean:
	rm -f $(TARGET)
	rm -rf logs/*
	rm -rf out/*
	@echo "Logs and target have been cleaned."
