# Final executable
out: main.o validate.o create.o update.o search.o display.o save.o func.o 
	gcc -o out main.o validate.o create.o update.o search.o display.o save.o func.o 

# Object file rules
main.o: main.c inverted.h
	gcc -c main.c

validate.o: validate.c inverted.h
	gcc -c validate.c

create.o: create.c inverted.h
	gcc -c create.c

update.o: update.c inverted.h
	gcc -c update.c

search.o: search.c inverted.h
	gcc -c search.c

display.o: display.c inverted.h
	gcc -c display.c

save.o: save.c inverted.h
	gcc -c save.c

func.o: func.c inverted.h
	gcc -c func.c

# Clean rule
clean:
	rm -f out *.o