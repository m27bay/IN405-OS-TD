CC = gcc
CFLAGS = -Wall -g -pthread
OFLAGS = -Ofast


reduc: clean
	$(CC) $(CFLAGS) $(OFLAGS) exo1.c -o reduction


fence: clean compile
	./main

compile: main.o exo2.o
	$(CC) $(CFLAGS) $(OFLAGS) main.o exo2.o -o main

main.o: main.c
	$(CC) $(CFLAGS) $(OFLAGS) -c main.c

exo2.o: exo2.c exo2.h
	$(CC) $(CFLAGS) $(OFLAGS) -c exo2.c exo2.h

mem_check: compile
	valgrind ./main

clean:
	rm -f *.o
	rm -f *.gch
	rm -f main
	rm -f reduction
	ls
