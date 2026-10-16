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
> @for f in tests/*.c; do \
>   n=$$(basename $$f); \
>   echo "== $$n"; \
>   ./$(TARGET) $$f build/$$n.html     || exit 1; \
>   ./$(TARGET) $$f build/$$n.n.html -n || exit 1; \
>   for h in build/$$n.html build/$$n.n.html; do \
>     a=$$(grep -o '<span' $$h | wc -l); \
>     b=$$(grep -o '</span>' $$h | wc -l); \
>     [ $$a -eq $$b ] || { echo "FAIL: unbalanced spans in $$h"; exit 1; }; \
>   done; \
> done; echo "All tests passed"

clean:
> rm -rf build

.PHONY: all test clean
