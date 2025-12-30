
CC = gcc

TARGET = main.out
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g

all: $(TARGET)

$(TARGET): main.c HashSet.c HashSet.h utils.c utils.h
	$(CC) -o $(TARGET) $(CFLAGS) main.c HashSet.c utils.c

.PHONY: HashSet.out
HashSet.out: HashSet.c HashSet.h utils.c utils.h
	$(CC) -o HashSet.out $(CFLAGS) HashSet.c utils.c

.PHONY: clean
clean:
	rm -rf *.out
