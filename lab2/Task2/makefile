all:main.exe

main.exe: task2.o
	gcc -m32 -g -Wall -o main.exe task2.o
task2.o: task2.c
	gcc -m32 -c -Wall -o task2.o task2.c
clean:
	rm -f *.o *.exe
debug:
	gcc -g -o debug task2.c
