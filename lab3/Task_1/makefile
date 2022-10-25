all:main.exe

main.exe: virusDetector.o
	gcc -m32 -g -Wall -o main.exe virusDetector.o
virusDetector.o: virusDetector.c
	gcc -m32 -c -Wall -o virusDetector.o virusDetector.c
clean:
	rm -f *.o *.exe
debug:
	gcc -g -o debug virusDetector.c
