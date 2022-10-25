all: skeleton

skeleton: skeleton.o
	gcc -m32 -g -Wall -o skeleton skeleton.o -nostdlib
skeleton.o: skeleton.s
	nasm -g -f elf -w+all -o skeleton.o skeleton.s

.PHONY: clean
clean:
		rm -f *.o skeleton