OBJ = a.out

OBJFILES = $(patsubst %.c,%.o,$(wildcard *.c))

$(OBJ): $(OBJFILES)
	gcc -o $@ $^

clean:
	rm -f *.o a.out