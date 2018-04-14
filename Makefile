#word: read.o word.o main.o
word: cells.o main.o
	gcc -Wall cells.o main.o -o word

#read.o: read.c
#	gcc -Wall read.c -c

#word.o: word.c
#	gcc -Wall word.c -c

cells.o: cells.c
	gcc -Wall cells.c -c

main.o: main.c
	gcc -Wall main.c -c

clean:
	rm -f *.o word
