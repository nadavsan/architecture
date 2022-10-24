all:main.exe

main.exe: addresses.o
	gcc -m32 -g -Wall -o main.exe addresses.o
addresses.o: addresses.c
	gcc -m32 -c -Wall -o addresses.o addresses.c
clean:
	rm -f *.o *.exe
debug:
	gcc -g -o debug addresses.c
