all: ruleone ruletwo rulethree rulefour
ruleone: start.s
	nasm -f elf32 start.s -o start.o
ruletwo: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
rulethree: main.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o
rulefour: start.o main.o util.o
	ld -m elf_i386 start.o main.o util.o -o task0
.PHONY: clean
clean:
	rm -f *.o task0