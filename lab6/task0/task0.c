#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "LineParser.h"

void mypipe(){
    int piped1[2];
    int p1;
    char str[] = "hello",buff[6];
    pipe(piped1);
       
    if((p1 = fork()) < 0)
    {
            perror("fork");
            exit(1);
    }

    if(p1 == 0){
        close(piped1[0]);/*Child process does not need this end of the pipe*/

        /* Send "string" through the output side of pipe */
        write(piped1[1], str, (sizeof(str)+1));
        fprintf(stdout,"child is exiting\n");
        exit(0);
    }
    else{
        fprintf(stdout,"parent is waiting for child to finish\n");
        wait(NULL); /*wait for child process to send the string*/
        fprintf(stdout,"parent stopped waiting\n");
        
        /* Parent process closes up output side of pipe */
        close(piped1[1]);/*Parent process does not need this end of the pipe*/

        /* Read in a string from the pipe */
        read(piped1[0], buff, sizeof(buff));
        printf("Read string: %s\n", buff);
    }
}

void mypipeline(){
    const char* lsc = "ls -l";
    const char* tailc = "tail -n 2";
    cmdLine* command;
    int piped1[2];
    int p1,p2, wefd,refd, argsret, argsret2, status1, status2;
    char str[] = "hello",buff[6];

    pipe(piped1);
       
    if((p1 = fork()) < 0){
            perror("fork");
            exit(1);
    }
    else if(p1 == 0){
        close(STDOUT_FILENO);
        wefd = dup(piped1[1]);
        close(piped1[1]);
        command = parseCmdLines(lsc);
        argsret = execvp(command->arguments[0],command->arguments);
        if(argsret == -1){
            perror("execv error");
            _exit(1);
            return;
        }
    }
    else{
        /* Parent process closes up output side of pipe */
        close(piped1[1]);/*Parent process does not need to write*/
        if((p2 = fork()) < 0){
            perror("fork");
            exit(1);
        }
        else if(p2 == 0) {
            close(STDIN_FILENO);
            refd = dup(piped1[0]);
            close(piped1[0]); /* child process does not need to read*/
            command = parseCmdLines(tailc);
            argsret2 = execvp(command->arguments[0],command->arguments);
            if(argsret2 == -1){
                perror("execv error");
                _exit(1);
                return;
            }
        }
        else{
            close(piped1[0]);
            waitpid(p1,&status1,0);
            waitpid(p2,&status2,0);
        }
        /*wait(NULL);*/ /*wait for child processes to terminate */  
        
    }
}

int main(int argc, char* argv[]){
    /*mypipe();*/
    mypipeline();
}