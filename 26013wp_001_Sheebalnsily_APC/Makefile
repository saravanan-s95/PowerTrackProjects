
CC = gcc
CFLAGS = -Wall -Wextra
TARGET = apc
SRCS = main.c apc.c
HEADERS = apc.h

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
