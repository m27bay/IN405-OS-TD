name_projet = search_atlacan

start: clean run

run: compile
	clear
	./a.out

compile:
	gcc atlacan.c search_atlacan.c

archive:
	rm -f $(name_projet).tar
	tar -cvf $(name_projet).tar README2.md makefile search_atlacan.c atlacan.c atlacan.h

clean:
	rm -f *.o
	rm -f a.out
	ls -l
