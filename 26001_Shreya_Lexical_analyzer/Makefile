CC = gcc

CFLAGS = -Wall

TARGET = ./lexical_analyzer

OBJ = main.o lexer.o 

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.phony: all clean