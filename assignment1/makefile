#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: task1 task2

# Tool invocations
task1: main_task1Assignment1.o asm_task1Assignment1.o
	gcc -m32 -g -Wall -o task1 main_task1Assignment1.o asm_task1Assignment1.o

# Depends on the source and header files
main_task1Assignment1.o: main_task1Assignment1.c
	gcc -g -Wall -m32 -ansi -c -o main_task1Assignment1.o main_task1Assignment1.c 
 
asm_task1Assignment1.o: asm_task1Assignment1.s
	nasm -g -f elf -w+all -o asm_task1Assignment1.o asm_task1Assignment1.s

# Tool invocations
task2: main_task2Assignment1.o asm_task2Assignment1.o
	gcc -m32 -g -Wall -o task2 main_task2Assignment1.o asm_task2Assignment1.o

# Depends on the source and header files
main_task2Assignment1.o: main_task2Assignment1.c
	gcc -g -Wall -m32 -ansi -c -o main_task2Assignment1.o main_task2Assignment1.c 
 
asm_task2Assignment1.o: asm_task2Assignment1.s
	nasm -g -f elf -w+all -o asm_task2Assignment1.o asm_task2Assignment1.s


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1