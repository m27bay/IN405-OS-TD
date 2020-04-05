run: compile
	./main2
	./main1

compile: main1.c main2.c
	gcc -Wall -g main1.c -o main1
	gcc -Wall -g main2.c -o main2

clean:
	rm -f pipe1
	rm -f main1
	rm -f main2
	ls
