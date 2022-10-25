#include <stdio.h>
#include <stdlib.h>


void printHex(char *buffer,int fpSize) {
  for(int i = 0;i < fpSize; i++)
    printf("%x ",*(buffer + i));
  printf("\n");
}

int main(int argc,char *argv[]) {

  FILE *fp; 
  char* name = argv[1];
  fp = fopen(name,"r");

  fseek(fp,0,SEEK_END);
  int fpSize = ftell(fp);
  fseek(fp,0,SEEK_SET);

  char *s = malloc(fpSize+1);
  // read first fpSize char
  int read = fread(s,sizeof(char),fpSize,fp);
  printHex(s,fpSize);
  fclose(fp);
  free(s);
  return 0;
}