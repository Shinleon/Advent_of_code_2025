
CC = gcc

TARGET = main.out
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

all: $(TARGET)

$(TARGET): main.c
	$(CC) -o $(TARGET) $(CFLAGS) main.c -lm

.PHONY: clean
clean:
	rm -rf $(TARGET)
