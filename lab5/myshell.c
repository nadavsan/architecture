#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "LineParser.h"
#include "linux/limits.h"

void execute(cmdLine *pCmdLine){
    int args_num, pid, status;
    if (strcmp(pCmdLine->arguments[0],"cd") == 0) {
        if (chdir(pCmdLine->arguments[1]) == -1){
            perror("cd error");
        }
        return;
    }
    if((pid = fork()) == 0){
        if (pCmdLine->blocking) { /* Task 1 */
            waitpid(pid, &status);
        }
        args_num = execvp(pCmdLine->arguments[0],pCmdLine->arguments);
        if(args_num == -1){
            perror("execv error");
            fprintf(stderr,"pid = %d, command = %s\n",pid,pCmdLine->arguments[0]);
            freeCmdLines(pCmdLine);
            _exit(1);
        }
        
    }
}

int isQuit(char *s){
    int i;
    for (i=0; i<strlen(s); i++){
        if (strncmp(s+i,"quit",4) == 0){
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv){
    char buf[2048];
    char *pathName = calloc(PATH_MAX,1);
    cmdLine* ans;

    while (1){
        getcwd(pathName,PATH_MAX);
        fprintf(stdout,"%s$ ",pathName);
        fgets(buf,sizeof(buf),stdin);
        if (isQuit(buf)){
            freeCmdLines(ans); 
            break;
        }
        ans = parseCmdLines(buf);
        execute(ans);
    }
    free(pathName);
    return 0;
}