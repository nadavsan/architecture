#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1
#define LSEEK 19
#define OPEN 5
#define CLOSE 6
#define SYS_READ 3

extern int system_call();

int main (int argc , char* argv[], char* envp[])
{
  int i, ptr, read;
  char buff[5];
  const char* shira = "Shira";
  char* fileName = argv[1];
  char X_NAME[sizeof(argv[2])+1];
  for ( i=0; i<sizeof(argv[2])+1; i++){
    X_NAME[i] = argv[2][i];
  }
  system_call(SYS_WRITE,STDOUT,"x_name=",7);
  system_call(SYS_WRITE,STDOUT,X_NAME,strlen(X_NAME));
  int fd = system_call(OPEN,fileName,0x002, 8);
  int fileLen = system_call(LSEEK,fd,0,2);
  ptr = system_call(LSEEK,fd,0,0);
  for(i = 0;i < sizeof(buff) && read >= 0; i++)
      read = system_call(SYS_READ,fd,&buff[i],1);
  /*print test*/
  system_call(SYS_WRITE,STDOUT,"filelen=",9);
  system_call(SYS_WRITE,STDOUT,itoa(fileLen),strlen(itoa(fileLen)));
  system_call(SYS_WRITE,STDOUT,"\n",1);

  while (ptr < fileLen-4){
    if(strncmp(buff,shira,5) == 0){
      ptr = system_call(LSEEK,fd,-1,1);
      system_call(SYS_WRITE,fd,&X_NAME,sizeof(X_NAME));
    }
    /* inc ptr before next interation:*/
    ptr = system_call(LSEEK,fd,0,1);
    /*print test*/
    system_call(SYS_WRITE,STDOUT,"ptr=",4);
    system_call(SYS_WRITE,STDOUT,itoa(ptr),strlen(itoa(ptr)));
    system_call(SYS_WRITE,STDOUT,"\n",1);

    for(i = 0;i < sizeof(buff) && read >= 0; i++)
      read = system_call(SYS_READ,fd,&buff[i],1);
    ptr = system_call(LSEEK,fd,-4,1);
  }
  int ret = system_call(CLOSE,fd);
  if(ret >= 0)
    return 1;
  return 0;
} 