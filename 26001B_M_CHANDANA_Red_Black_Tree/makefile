CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

TARGET = rbt

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c main.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)