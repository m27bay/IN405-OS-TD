run: compile
	./main

compile: main.o process.o
	gcc -Wall -g main.o process.o -o main

process.o: process.c process.h
	gcc -Wall -g -c process.c

main.o: main.c
	gcc -Wall -g -c main.c

clean:
	rm -f *.o
	rm -f main
	ls
