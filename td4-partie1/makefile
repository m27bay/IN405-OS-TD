start: clean run

check_mem: compile
	valgrind ./main

debug: compile
	gdb ./main

run: compile
	./main

compile: main.o stat.o
	clear
	gcc -Wall -g main.o stat.o -o main

main.o: main.c
	gcc -Wall -g -c main.c

stat.o: stat.c stat.h
	gcc -Wall -g -c stat.c

clean:
	rm -f *.o
	rm -f main
	ls
