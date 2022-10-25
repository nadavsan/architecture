all: looper

myshell: myshell.o LineParser.o 
	gcc -m32 -g -Wall -o myshell myshell.o LineParser.o 
	
myshell.o: myshell.c
	gcc -g -Wall -m32 -c -o myshell.o myshell.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c

looper: looper.o
	gcc -m32 -g -Wall -o looper looper.o

looper.o: looper.c
	gcc -g -Wall -m32 -c -o looper.o looper.c
	
clean: 
	rm -f *.o myshell