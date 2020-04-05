mem_check: compile
	valgrind ./main

run: compile
	./main

compile: main.o td7.o
	gcc -Wall -g main.o td7.o -o main

main.o: main.c
	gcc -Wall -g -c main.c

td7.o: td7.c td7.h
	gcc -Wall -g -c td7.c td7.h

clean:
	rm -f *.o
	rm -f *.gch
	rm -f main
	ls
