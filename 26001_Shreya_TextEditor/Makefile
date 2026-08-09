CC = gcc

CFLAGS = -Wall

TARGET = txt_editor.out

OBJ = text_editor.o text_ops.o undo_redo.o save_to_file.o save_and_exit.o navigation.o load_from_file.o display.o

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean :
	rm -f $(OBJ) $(TARGET)

.PHONY : all clean
