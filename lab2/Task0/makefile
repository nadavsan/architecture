all:main.exe

main.exe: count-words.o
	gcc -g -Wall -o main.exe count-words.o
count-words.o: count-words.c
	gcc -c -Wall -o count-words.o count-words.c
clean:
	rm -f *.o *.exe
debug:
	gcc -g -o debug count-words.c