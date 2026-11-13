
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g
TARGET = text_editor
SOURCES = main.c text_ops.c file_ops.c display_search.c history.c

all: $(TARGET)

$(TARGET): $(SOURCES) text_editor.h editor_internal.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
