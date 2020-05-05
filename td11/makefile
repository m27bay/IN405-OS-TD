CC = gcc
CFLAGS = -Wall -g -pthread
OFLAGS = -Ofast
LIBFLAGS = -L. -liof

dataFile = memory.cfg
dataFile2 = memory2.cfg
dataFile3 = memory3.cfg

program = makefile memory.c
library = libiof.a se_fichier.h types.h
config = memory.cfg memory2.cfg memory3.cfg

run: compile
	./memory $(dataFile)

run2: compile
	./memory $(dataFile2)

run3: compile
	./memory $(dataFile3)

compile: clean
	$(CC) $(CFLAGS) memory.c $(LIBFLAGS) $(OFLAGS) -o memory


memCheck: cleanTube
	valgrind ./memory $(dataFile)

memCheck2: cleanTube
	valgrind ./memory $(dataFile2)

memCheckFull: cleanTube
	valgrind --leak-check=full ./memory $(dataFile)

memCheckFull2: cleanTube
	valgrind --leak-check=full ./memory $(dataFile2)

memCheckOrigine2: cleanTube
	valgrind --track-origins=yes ./memory $(dataFile2)


cleanArchive:
	rm -f LE_DENMAT_Mickeal.tar

	rm -f td11/*
	rm -df td11

archive: cleanArchive
	mkdir td11
	cp -u README.md $(program) $(library) $(config) td11/

	tar -cvf LE_DENMAT_Mickeal.tar td11/


cleanTube:
	rm -f tubeThreads
	rm -f tubeFather

clean: cleanTube
	rm -f memory
	ls
