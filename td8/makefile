CC = gcc

CFLAGS = -Wall -g

run: compile server client


server:
	./million server lottery.cfg &

client:
	./million client 2 5 9 12 15


mem_check:
	valgrind make

mem_check2:
	valgrind --leak-check=full make


compile: clean million.o se_fichier.o
	$(CC) $(CFLAGS) million.o se_fichier.o -o million
	clear


million.o: million.c
	$(CC) $(CFLAGS) -c million.c

se_fichier.o: se_fichier.c se_fichier.h
	$(CC) $(CFLAGS) -c se_fichier.c se_fichier.h


clean_archive:
	rm LE_DENMAT_Mickeal.tar

	rm Server_Client/*
	rmdir Server_Client

archive: clean_archive
	mkdir Server_Client
	cp -u README.md makefile million.c scenario-test.sh lottery.cfg se_fichier.c se_fichier.h Server_Client/

	tar -cvf LE_DENMAT_Mickeal.tar Server_Client/


clean_tube:
	rm -f /tmp/tube-cli
	rm -f /tmp/tube-serv

clean: clean_tube
	rm -f *.o
	rm -f *.gch
	rm -f million
	ls
