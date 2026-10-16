.RECIPEPREFIX = >
CC     = gcc
CFLAGS = -Wall -Wextra -Iinc
SRC    = $(wildcard src/*.c)
OBJ    = $(SRC:src/%.c=build/%.o)
TARGET = build/lexer

all: $(TARGET)

$(TARGET): $(OBJ)
> $(CC) $(OBJ) -o $@

build/%.o: src/%.c inc/lexer.h | build
> $(CC) $(CFLAGS) -c $< -o $@

build:
> mkdir -p build

test: $(TARGET)
> @for f in tests/t*.c; do echo "== $$f"; ./$(TARGET) $$f; done

clean:
> rm -rf build

.PHONY: all test clean
