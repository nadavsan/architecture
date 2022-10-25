all:main.exe

main.exe: hexaPrint.o
	gcc -m32 -g -Wall -o main.exe hexaPrint.o
hexaPrint.o: hexaPrint.c
	gcc -m32 -c -Wall -o hexaPrint.o hexaPrint.c
clean:
	rm -f *.o *.exe
debug:
	gcc -g -o debug hexaPrint.c
