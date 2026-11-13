CC = gcc

CFLAGS = -Wall -Wextra -g

TARGET = inverted_search

SRCS = main.c \
       create_database.c \
       display_database.c \
       file_validation.c \
       insert_last.c \
       save_database.c \
       search.c \
       update_database.c

OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c inverted_Search.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)