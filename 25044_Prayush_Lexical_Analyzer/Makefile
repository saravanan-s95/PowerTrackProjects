CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

SRCS = src/main.c src/tokenizer.c src/validations.c src/token_handler.c
OBJS = $(SRCS:.c=.o)
TARGET = Lexical_Analyzer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
