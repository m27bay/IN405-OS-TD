CC = gcc
CFLAGS = -Wall -g -pthread
OFLAGS = -Ofast

exo1: compile
	./main

compile: main.o thread.o
	$(CC) $(CFLAGS) $(OFLAGS) main.o thread.o -o main
# 	clear

main.o: main.c
	$(CC) $(CFLAGS) $(OFLAGS) -c main.c

thread.o: thread.c thread.h
	$(CC) $(CFLAGS) $(OFLAGS) -c thread.c thread.h -pthread


reduc:
	$(CC) $(CFLAGS) $(OFLAGS) reduc.c -o reduction


mem_check: compile
	valgrind ./main

clean:
	rm -f *.o
	rm -f *.gch
	rm -f main
	rm -f reduction
	ls
