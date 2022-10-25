all: myshell
task0: task0.o
	gcc -m32 -g -Wall -o task0 task0.o LineParser.o
task0.o: task0.c
	gcc -g -Wall -m32 -c -o task0.o task0.c
	
myshell: myshell.o LineParser.o 
	gcc -m32 -g -Wall -o myshell myshell.o LineParser.o 
	
myshell.o: myshell.c
	gcc -g -Wall -m32 -c -o myshell.o myshell.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c
	
clean: 
	rm -f *.o myshell