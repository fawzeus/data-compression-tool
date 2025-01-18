# The compiler: gcc for C program
CC = gcc

# Compiler flags:
#  -g     - adds debugging information to the executable file
#  -Wall  - turns on most compiler warnings
CFLAGS  = -Wall

SOURCE = src/*.c

# The build target
TARGET = build/compressor

all: $(TARGET)
	@echo "\n Build completed successfully! Run ./$(TARGET) to execute your program!"

$(TARGET): $(SOURCE)
	$(CC) -o $(TARGET) $(SOURCE) $(CFLAGS)

clean:
	rm -f $(TARGET)
