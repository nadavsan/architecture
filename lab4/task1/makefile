all: ruleone ruletwo rulethree rulefour
ruleone: start.s
	nasm -f elf32 start.s -o start.o
ruletwo: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
rulethree: count.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector count.c -o count.o
rulefour: start.o count.o util.o
	ld -m elf_i386 start.o count.o util.o -o count
.PHONY: clean
clean:
	rm -f *.o count