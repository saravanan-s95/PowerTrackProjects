CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic
TARGET = rbt
SOURCES = main.c rbt.c insert.c delete.c search.c display.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

%.o: %.c rbt.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
