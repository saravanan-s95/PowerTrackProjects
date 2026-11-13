
CC = gcc
CFLAGS = -Wall -Wextra
TARGET = s2html
SRCS = main.c parser.c source_to_html.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
