CC=gcc

CFLAGS=-Wall

OBJ=main.o lexer.o html.o keywords.o

source2html: $(OBJ)
	$(CC) $(OBJ) -o source2html

main.o: main.c source2html.h
	$(CC) $(CFLAGS) -c main.c

lexer.o: lexer.c source2html.h
	$(CC) $(CFLAGS) -c lexer.c

html.o: html.c source2html.h
	$(CC) $(CFLAGS) -c html.c

keywords.o: keywords.c source2html.h
	$(CC) $(CFLAGS) -c keywords.c

clean:
	rm -f *.o source2html output.html