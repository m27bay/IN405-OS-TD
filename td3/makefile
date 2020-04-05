start: clean_file clean run open_file
start2: clean_file2 clean run2 open_file2

run: compile
	./main

compile: main.o io_file.o
	gcc -Wall -g main.o io_file.o -L. -liof -o main

main.o: main.c
	gcc -Wall -g -c main.c

io_file.o:
	gcc -Wall -c -fPIC io_file.c
	ar rcs libiof.a io_file.o

mem_check: compile2
	valgrind ./main_opt

mem_check2: compile2
	valgrind --track-origins=yes ./main_opt

mem_check3: compile2
	valgrind --leak-check=full ./main_opt

debug: compile2
	gdb ./main_opt

run2: compile2
	./main_opt

compile2: main_opt.o io_file.o
	gcc -Wall -g main_opt.o io_file.o -L. -liof -o main_opt

main_opt.o: main_opt.c
	gcc -Wall -g -c main_opt.c

clean_file:
	clear
	rm -f *.txt *.log
	ls

clean_file2:
	clear
	rm -f *.txt
	ls

open_file:
	sublime *.txt *.log

open_file2:
	sublime *.txt

clean:
	rm -f *.o
	rm -f main
	rm -f main_opt
	ls
