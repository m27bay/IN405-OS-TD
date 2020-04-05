MemCheck = --leak-check=full
MemCheck2 = $(MemCheck) --show-leak-kinds=all

all: run
	./lexc

run: main.o lexc.o menu_lexc.o
	gcc -Wall main.o lexc.o menu_lexc.o -o lexc

main.o: main.c
	gcc -Wall -g main.c -c

lexc.o: lexc.c lexc.h
	gcc -Wall -g lexc.c -c

menu_lexc.o: menu_lexc.c menu_lexc.h
	gcc -Wall -g menu_lexc.c -c

debug: run
	valgrind ./lexc

debug2: run
	valgrind $(MemCheck) ./lexc

debug3: run
	valgrind $(MemCheck2) ./lexc

clean:
	rm -f *.o
	rm -f lexc
	ls
