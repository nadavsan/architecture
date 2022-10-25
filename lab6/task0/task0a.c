#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

void mypipe(){
    int _pipedes[2];
    int p;
    char str[] = "hello",buff[6];
    pipe(_pipedes);
       
    if((p = fork()) < 0)
    {
            perror("fork");
            exit(1);
    }

    if(p == 0){
        close(_pipedes[0]);/*Child process does not need this end of the pipe*/

        /* Send "string" through the output side of pipe */
        write(_pipedes[1], str, (sizeof(str)+1));
        fprintf(stdout,"child is exiting\n");
        exit(0);
    }
    else{
        fprintf(stdout,"parent is waiting for child to finish\n");
        wait(NULL); /*wait for child process to send the string*/
        fprintf(stdout,"parent stopped waiting\n");
        
        /* Parent process closes up output side of pipe */
        close(_pipedes[1]);/*Parent process does not need this end of the pipe*/

        /* Read in a string from the pipe */
        read(_pipedes[0], buff, sizeof(buff));
        printf("Read string: %s\n", buff);
    }
}

int main(int argc, char* argv[]){
    mypipe();
}