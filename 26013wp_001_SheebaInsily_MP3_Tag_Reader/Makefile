
CC = gcc
CFLAGS = -Wall -Wextra
TARGET = mp3_tag_reader
SRCS = main.c mp3_tag_reader.c
HEADERS = mp3_tag_reader.h types.h

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
