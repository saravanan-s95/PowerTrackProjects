
rb_tree: main.c rb_tree.c rb_tree.h
	gcc -Wall -Wextra main.c rb_tree.c -o rb_tree

clean:
	rm -f rb_tree
