CC = gcc

CFLAGS = -Wall

TARGET = mp3_tag_reader.out

OBJ = main.o check_version.o viewmp3.o editmp3.o 

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean :
	rm -f $(OBJ) $(TARGET)

.PHONY : all clean