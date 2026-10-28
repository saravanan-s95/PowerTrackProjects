CC = gcc
CFLAGS = -Wall -Wextra
TARGET = text_editor
SOURCES = $(wildcard *.c)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
