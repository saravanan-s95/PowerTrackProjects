CC = gcc
CFLAGS = -Wall -Wextra -I.
TARGET = lexer
SOURCES = main.c lexer.c
HEADERS = lexer.h

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean
