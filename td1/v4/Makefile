ListeWord = This is a string very long
MemCheck = --leak-check=full
MemCheck2 = $(MemCheck) --show-leak-kinds=all

gab: run_g
	./lexc_g

run_g: lexc_test_g.o lexc_g.o
	gcc -Wall lexc_test_g.o lexc_g.o -o lexc_g

lexc_test_g.o: lexc_test_g.c
	gcc -Wall -g lexc_test_g.c -c

lexc_g.o: lexc_g.c lexc_g.h
	gcc -Wall -g lexc_g.c -c

debug_g: run_g
	valgrind ./lexc_g

debug2_g: run_g
	valgrind $(MemCheck) ./lexc_g

debug3_g: run_g
	valgrind $(MemCheck2) ./lexc_g

clean:
	rm -f *.o
	rm -f lexc_g
	ls -l
