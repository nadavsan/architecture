#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "LineParser.h"
#include <linux/limits.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0

typedef struct process{
cmdLine* cmd; /* the parsed command line*/
pid_t pid; /* the process id that is
running the command*/
int status; /* status of the process:
RUNNING/SUSPENDED/TERMINATED */
struct process *next; /* next process in chain */
} process;

process** procs;

static int childProc = 0; /* whether it is a child process relative to main() */

void freeProcessList(process* process_list){
    process* temp;
    while (process_list != NULL){
        temp = process_list;
        process_list = process_list->next;
        free(temp);
    }
}

void updateProcessList(process **process_list){
    process* curr = process_list[0];
    pid_t cpid;
    int status;
    while (curr->next != NULL){
        cpid = fork();
        if (cpid == 0) {
            updateProcessStatus(procs,cpid,1);
        }
        else if (cpid == -1) {
            perror("could not fork properly");
            freeProcessList(process_list[0]);
            exit(1);
        }
         /* cpid is the father process */
        else if(waitpid(cpid, &status, WNOHANG | WUNTRACED | WCONTINUED) != 0){
            if (WIFSTOPPED(status)){
                updateProcessStatus(procs,cpid,SUSPENDED);
            }
            else if(WIFCONTINUED(status)){
                updateProcessStatus(procs,cpid,RUNNING);
            }
            else if (WIFEXITED(status) || WIFSIGNALED(status)){
                updateProcessStatus(procs,cpid,TERMINATED);
            }
        }
        curr = curr->next;  
    }
    
}

void updateProcessStatus(process* process_list, int pid, int status){
    process* curr = process_list;
    while (curr != NULL){
        if (curr->pid == pid) {
            curr->status = status;
        }
        curr = curr->next;
    }
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process* curr = process_list[0];
    while (curr->next != NULL){
        curr = curr->next;
    }
    struct process *last = calloc(1,sizeof(process));
    last->cmd = cmd;
    last->pid = pid;
    last->status = SUSPENDED;
    last->next = NULL;
    curr->next = last;
}

void delete_process(process* process_list,pid_t pid){
    process* curr = process_list;
    process* temp;
    int firstRound = 1;
    while(curr != NULL){
        if(firstRound){ /* first of the list is toDelete*/
            if(curr->pid == pid){
                process_list = process_list->next;
                free(curr);
                return;
            }
            curr = curr->next;
            firstRound = 0;
        }
        else if(curr->next!=NULL){ /* somewhere in the middle of the list is toDelete */
            if((curr->next)->pid == pid){
                temp = curr->next;
                curr->next = temp->next;
                free(temp);
            }
        }
        else if ((curr->next)->next == NULL && (curr->next)->pid == pid) { /* last of the list is toDelete */
            temp = curr->next;
            (curr->next) = NULL;
            free(temp);
        }
    }
}

void printProcess(process* p){
    printf("%d   %s   %d\n",p->pid,p->cmd->arguments[0], p->status);
}

void printProcessList(process** process_list){
    updateProcessList(process_list);
    process* curr = process_list[0];
    printf("%s   %s   %s\n","PID","Command","STATUS");
    while (curr != NULL) {
        printProcess(curr);
        if (curr->status==TERMINATED){
            delete_process(process_list[0],curr->pid);
        }
        curr = curr->next;
    }
}

void execute(cmdLine *pCmdLine){
    int args_num, pid, status;
    
    if (strcmp(pCmdLine->arguments[0],"cd") == 0) {
        if (chdir(pCmdLine->arguments[1]) == -1){
            perror("cd error");
        }
        return;
    }
    else if (strcmp(pCmdLine->arguments[0],"showprocs")==0){
        printProcessList(procs);
    }
    else if (strcmp(pCmdLine->arguments[0],"nap") == 0) {
        if ((pid = fork()) == 0) {
            kill(pid,SIGTSTP);
            sleep(atoi(pCmdLine->arguments[1]));
            kill(pid,SIGCONT); 
        }  
    }
    else if (strcmp(pCmdLine->arguments[0],"stop") == 0) {
        kill((pid_t)pCmdLine->arguments[1],SIGINT);
    }
    else if((pid = fork()) == 0){
        args_num = execvp(pCmdLine->arguments[0],pCmdLine->arguments);
        if(args_num == -1){
            perror("execv error");
            fprintf(stderr,"pid = %d, command = %s\n",pid,pCmdLine->arguments[0]);
            freeCmdLines(pCmdLine);
            _exit(1);
            return;
        }
        freeCmdLines(pCmdLine);
        return;
    }
    addProcess(procs,pCmdLine,pid);
    if (pCmdLine->blocking) { /* Task 1 */
        waitpid(pid, &status,0);
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

static void redirect(int oldfd, int newfd) {
  if (oldfd != newfd) {
    if (dup2(oldfd, newfd) != -1)
      close(oldfd); /* successfully redirected */
    else{
       perror("dup2");
       exit(1);
    }
  }
}

static void mypipeline(cmdLine *command, int in_fd) {
  if (command->next == NULL) { /* last command */
    redirect(in_fd, STDIN_FILENO); /* read from in_fd, write to STDOUT */
    execvp(command->arguments[0], command->arguments);
    perror("execvp last");
    exit(1);
  }
  else { /* $ <in_fd command[pos] >fd_pipe[1] | <fd_pipe[0] command[pos+1] ... */
    int fd_pipe[2]; /* output pipe */
    if (pipe(fd_pipe) == -1){
      perror("pipe");
      exit(1);
    }
    switch(fork()) {
    case -1:
      perror("fork");
      exit(1);
    case 0: /* child: execute current command `command[pos]` */
      childProc = 1;
      close(fd_pipe[0]); /* unused */
      redirect(in_fd, STDIN_FILENO);  /* read from in_fd */
      redirect(fd_pipe[1], STDOUT_FILENO); /* write to fd_pipe[1] */
      execvp(command->arguments[0], command->arguments);
      perror("execvp");
      _exit(1);
    default: /* parent: execute the rest of the commands */
      close(fd_pipe[1]); /* unused */
      close(in_fd); /* unused */
      mypipeline(command->next, fd_pipe[0]); /* execute the rest */
    }
  }
}

int firstIndexOf(char *s, char c){
    int i;
    for (i=0; i<strlen(s); i++){
        if (s[i] == c){
            return i;
        }
    }
    return -1; 
}

int firstIndexOfI(char *s, char c, int i){
    int j;
    for (j=i; j<strlen(s); j++){
        if (s[j] == c){
            return j;
        }
    }
    return -1; 
}

char* substr(const char *src, int m, int n)
{
    int len = n - m;
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
     *dest = '\0';
     return dest - len;
}

void executeCmdLine(cmdLine* pCmdLine){
    int argsret;
    if(pCmdLine->inputRedirect != NULL){
        fclose(stdin);
        fopen(pCmdLine->inputRedirect,"a+");
    }
    if(pCmdLine->outputRedirect != NULL){
        fclose(stdout);
        fopen(pCmdLine->outputRedirect,"a+");
    }
    argsret = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
    if(argsret == -1){
        perror("Execvp error");
        freeCmdLines(pCmdLine);
        _exit(1);
    }
}

int main(int argc, char **argv){
    char buf[2048];
    char *pathName = calloc(PATH_MAX,1);
    int pipe_ptr, inp_ptr, outp_ptr,pid3;
    cmdLine* ans;
    process* p = malloc(sizeof(process));
    p->cmd = parseCmdLines("./myshell");
    p->pid = getpid();
    p->status = 1;
    p->next = NULL;
    procs = &p;
    while (1){
        getcwd(pathName,PATH_MAX);
        fprintf(stdout,"%s$ ",pathName);
        fgets(buf,sizeof(buf),stdin);
        if (isQuit(buf)){
            freeCmdLines(ans); 
            break;
        }
        else if(-1 == (pipe_ptr = firstIndexOf(buf,'|'))){
            ans = parseCmdLines(buf);
            execute(ans);
        }
        /*else if (-1 != firstIndexOf(buf,'<') || -1 != firstIndexOf(buf,'>')) {
            if ((pid3 = fork()) == 0)
            {
                executeCmdLine(parseCmdLines(buf));
            }
        }*/
        else {
            if((pid3 = fork()) == -1){
                perror("fork");
                exit(1);
            }
            else if( pid3 == 0){
                ans = parseCmdLines(buf);
                mypipeline(ans,STDIN_FILENO);
            }
        }
    }
    free(p);
    free(pathName);
    return 0;
}