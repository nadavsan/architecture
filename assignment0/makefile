compC = gcc -m32 -g -Wall -c -o
compS = nasm -g -f elf -w+all -o

.PHONY : all clean

all: ass0

ass0 : mainAssignment0.o asmAssignment0.o
	gcc -m32 -g -Wall -o ass0 mainAssignment0.o asmAssignment0.o

mainAssignment0.o : mainAssignment0.c 
	$(compC) mainAssignment0.o mainAssignment0.c

asmAssignment0.o : asmAssignment0.s
	$(compS) asmAssignment0.o asmAssignment0.s


clean:
	rm -f *.o ass0
