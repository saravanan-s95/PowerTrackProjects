CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
TARGET = text_editor
SOURCES = text_editor.c

all: $(TARGET)

$(TARGET): $(SOURCES) text_editor.h file_ops.h text_ops.h cursor_navigations.h display_search.h redo_undo.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)


