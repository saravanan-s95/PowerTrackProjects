a.out:main.o graph.o storage.o validation.o
	gcc -o a.out main.o graph.o storage.o validation.o
main.o: main.c graph.h storage.h validation.h
	gcc -c main.c
graph.o: graph.c graph.h
	gcc -c graph.c
storage.o: storage.c storage.h graph.h
	gcc -c storage.c
validation.o: validation.c validation.h
	gcc -c validation.c
clean:
	rm *.out *.o