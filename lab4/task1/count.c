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

extern int system_call();

void exercise1b(int argc, char* argv[]){
    int fdi, fdo, i, debug = 0, input = 0, output = 0;
    char* tmp;
    for(i = 1;i < argc; i++){
        if (argv[i][0]=='-' && argv[i][1]=='D'){
            debug = 1;
        }
        else if(argv[i][0]=='-' && argv[i][1] == 'i'){
            input = 1;
            fdi = system_call(OPEN,argv[i]+2,O_RDONLY);
        }
        else if(argv[i][0]=='-' && argv[i][1] == 'o'){
            output = 1;
            tmp = argv[i] + 2;
            fdo = system_call(OPEN,tmp,O_WRONLY|O_APPEND|O_CREAT,O_RWX);
        }
    }

    if(!input)
        fdi = STDIN;
    if(!output)
        fdo = STDOUT;
    system_call(SYS_WRITE,STDERR,"fdo=",4);
    system_call(SYS_WRITE,STDOUT,itoa(fdo),strlen(itoa(fdo)));
    system_call(SYS_WRITE,STDOUT,"\n",1);
    if (!debug) {
        char buf;
        int counter=1; 
        while (system_call(SYS_READ,fdi,&buf,1) >= 0) {
            if(buf == '\n')
                break;
            if(buf == ' ')
                counter++;
            while (buf == ' ') {
                system_call(SYS_READ,fdi,&buf,1);
            }
            
        }
        system_call(SYS_WRITE,fdo,itoa(counter),strlen(itoa(counter)));
        system_call(SYS_WRITE,fdo,"\n",1);
    }else {
        char buf;
        int ans;
        int counter=1; 
        while ((ans = system_call(SYS_READ,fdi,&buf,1)) >= 0) {
            system_call(SYS_WRITE,STDERR,"3  ",3);
            system_call(SYS_WRITE,STDERR,itoa(ans),strlen(itoa(ans)));
            system_call(SYS_WRITE,STDERR,"\n",1);
            if(buf == '\n')
                break;
            if(buf == ' ')
                counter++;
            while (buf == ' ') {
                ans = system_call(SYS_READ,fdi,&buf,1);
                system_call(SYS_WRITE,STDERR,"3  ",3);
                system_call(SYS_WRITE,STDERR,itoa(ans),strlen(itoa(ans)));
                system_call(SYS_WRITE,STDERR,"\n",1);
            }

        }
        ans = system_call(SYS_WRITE,fdo,itoa(counter),strlen(itoa(counter)));
        system_call(SYS_WRITE,fdo,"\n",1);
        system_call(SYS_WRITE,STDERR,"1  ",3);
        system_call(SYS_WRITE,STDERR,itoa(ans),strlen(itoa(ans)));
        system_call(SYS_WRITE,STDERR,"\n",1);
    }
}

void exercise1a(int argc, char* argv[]){
    if (argc == 1) {

    char buf;
    int counter=1; 
    while (system_call(SYS_READ,STDIN,&buf,1) >= 0) {
        if(buf == '\n')
            break;
        if(buf == ' ')
            counter++;
        while (buf == ' ')
            system_call(SYS_READ,STDIN,&buf,1);
        
    }
    system_call(SYS_WRITE,STDOUT,itoa(counter),strlen(itoa(counter)));
    system_call(SYS_WRITE,STDOUT,"\n",1);
    }
    else {
        char buf;
        int ans;
        int counter=1; 
        while ((ans = system_call(SYS_READ,STDIN,&buf,1)) >= 0) {
            system_call(SYS_WRITE,STDERR,"3  ",3);
            system_call(SYS_WRITE,STDERR,itoa(ans),strlen(itoa(ans)));
            system_call(SYS_WRITE,STDERR,"\n",1);
            if(buf == '\n')
                break;
            if(buf == ' ')
                counter++;
            while (buf == ' ') {
                ans = system_call(SYS_READ,STDIN,&buf,1);
                system_call(SYS_WRITE,STDERR,"3  ",3);
                system_call(SYS_WRITE,STDERR,itoa(ans),strlen(itoa(ans)));
                system_call(SYS_WRITE,STDERR,"\n",1);
            }

        }
        ans = system_call(SYS_WRITE,STDOUT,itoa(counter),strlen(itoa(counter)));
        system_call(SYS_WRITE,STDOUT,"\n",1);
        system_call(SYS_WRITE,STDERR,"1  ",3);
        system_call(SYS_WRITE,STDERR,itoa(ans),strlen(itoa(ans)));
        system_call(SYS_WRITE,STDERR,"\n",1);
    }
}

int main (int argc , char* argv[], char* envp[])
{
    /*exercise1a(argc,argv);*/
    exercise1b(argc,argv);
    return 0;
}