CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = a.out

SOURCES = $(wildcard *.c)

OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c rbt.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) a.out