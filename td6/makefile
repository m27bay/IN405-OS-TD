mem_check: compile
	valgrind ./main

run: compile
	./main

compile: main.o process2.o
	gcc -Wall -g main.o process2.o -o main

main.o: main.c
	gcc -Wall -g -c main.c

process2.o: process2.c process2.h
	gcc -Wall -g -c process2.c process2.h

clean:
	rm -f *.o
	rm -f *.gch
	rm -f main
	ls
