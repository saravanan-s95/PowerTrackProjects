editor : main.o text_editor.o text_ops.o cursor_nav.o display.o redo_undo.o
	gcc -Wall -g -o editor main.o text_editor.o text_ops.o cursor_nav.o display.o redo_undo.o

main.o : main.c
	gcc -Wall -g -c main.c

text_editor.o : text_editor.c
	gcc -Wall -g -c text_edditor.c

text_ops.o : text_ops.c
	gcc -Wall -g -c teat_ops.c

cursor_nav.o : cursor_nav.c
	gcc -Wall -g -c cursor_nav.c

display.o : display.c
	gcc -Wall -g -c display.c

redo_undo.o : redo_undo.c
	gcc -Wall -g -c redo_undo.c

clean :
	rm -f *.o editor				



