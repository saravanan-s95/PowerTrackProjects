CC = gcc
CFLAGS = -Wall -Wextra
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = minishell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
