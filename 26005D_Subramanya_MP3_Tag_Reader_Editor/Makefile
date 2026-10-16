CC     = gcc
CFLAGS = -Wall -Wextra -g
TARGET = a.out
OBJDIR = obj

SRCS = mp3_main.c mp3_view.c mp3_edit.c functions.c
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(OBJDIR)/%.o: %.c mp3_header.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET) temp.mp3

.PHONY: all clean
