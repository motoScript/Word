word: word.o main.o
	gcc -Wall word.o main.o -o word

word.o: word.c
	gcc -Wall word.c -c

main.o: main.c
	gcc -Wall main.c -c

clean:
	rm -f *.o word
