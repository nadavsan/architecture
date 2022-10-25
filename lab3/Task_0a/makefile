all:main.exe

main.exe: lab3_mergesort.o
	gcc -m32 -g -Wall -o main.exe lab3_mergesort.o
lab3_mergesort.o: lab3_mergesort.c
	gcc -m32 -c -Wall -o lab3_mergesort.o lab3_mergesort.c
clean:
	rm -f *.o *.exe
debug:
	gcc -g -o debug lab3_mergesort.c
