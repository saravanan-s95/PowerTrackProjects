text_editor.out : text_editor.o text_ops.o curser_navigations.o file_ops.o display_search.o redo_undo.o
	gcc -o text_editor.out text_editor.o text_ops.o curser_navigations.o file_ops.o display_search.o redo_undo.o

text_editor.o : text_editor.c text_editor.h
	gcc -c text_editor.c

text_ops.o : text_ops.c text_ops.h text_editor.h
	gcc -c text_ops.c

curser_navigations.o : curser_navigations.c curser_navigations.h text_editor.h
	gcc -c curser_navigations.c

file_ops.o : file_ops.c file_ops.h text_editor.h
	gcc -c file_ops.c

display_search.o : display_search.c display_search.h text_editor.h
	gcc -c display_search.c

redo_undo.o : redo_undo.c redo_undo.h text_editor.h
	gcc -c redo_undo.c

clean :
	rm *.o text_editor.out