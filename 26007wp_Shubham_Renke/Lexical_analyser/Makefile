
CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = lexer
SOURCES = main.c analyzer.c token.c error.c
HEADERS = analyzer.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) sample.c

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
