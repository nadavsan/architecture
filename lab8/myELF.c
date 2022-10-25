#include <stdlib.h>
#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define OFFSET_ENTRY 0x18
#define OFFSET_SHDT 0x20
#define OFFSET_NUMBER_OF_SHDR_ENTRIES 0x30
#define OFFSET_SIZE_OF_SECTION_HEADER_ENTRY 0x2E
#define OFFSET_PHDT 0x1C
#define OFFSET_NUMBER_OF_PHD_ENTRIES 0x2c
#define OFFSET_SIZE_OF_PROGRAM_HEADER_ENTRY 0x2A
#define INDEX_0F_THE_SECTION_HEADER_TABLE_ENTRY_THAT_CONTAINS_THE_SECTION_NAME 0x32


typedef struct FUNCS{
  char* name;
  void (*func)();
} function;

void* map_start;
char debug;
int currfd=-1;
int fileSize;

void initMenu();
void toggleDebugMode();
void quit();
void examineELFFile();
void printSectionNames();
void printSymbols();
int findSymbolTableEntryInSectionHeaderTable(void* map_start);

int main(int argc, char* argv[]){
  function menu[]={
    {"Toggle Debug Mode",toggleDebugMode},
    {"Examine ELF File",examineELFFile},
    {"Print Section Names",printSectionNames},
    {"Print Symbols",printSymbols},
    {"Quit",quit},
    {NULL, NULL}    
  };
  int i;
  for(;;){
    printf("Choose Action:");
    printf("\n");
    for(i=0;menu[i].name!=NULL;i++){
      fprintf(stdout,"%d-%s",i,menu[i].name);
      fprintf(stdout,"\n");
    }
    char* c=malloc(10);
    fgets(c,10,stdin);
    int num=atoi(c);
    free(c);
    if(num>=0&&num<i)
      menu[num].func();
    printf("\n");
  }
  
  return 0;
}

void toggleDebugMode(){
  if(debug){    
    debug=0;
    fprintf(stdout,"Debug flag now off\n");
  }
  else{
    debug=1;
    fprintf(stdout,"Debug flag now on\n");
  }
}
void quit(){
  if(debug)
    fprintf(stderr,"Quitting...\n");
  if(map_start!=NULL)
    munmap(map_start,fileSize);
  if(currfd!=-1)
    close(currfd);  
  exit(0);
}
void examineELFFile(){
  /* Getting The File */
  char input_name[1024];
  printf("enter file name: ");
  fgets(input_name,1024,stdin);
  char *ptr;
  if ((ptr=strchr(input_name, '\n')) != NULL)
    *ptr = '\0';
  if(debug)
    fprintf(stderr,"Debug: file name was changed to: %s\n",input_name);
  /* checks if a file is already open */
  if(currfd!=-1)
    close(currfd);
  currfd=open(input_name,0,S_IRUSR);
  if(currfd==-1){
    perror(input_name);
    return;
  }
  /* size of file */
  fileSize=lseek(currfd,0,SEEK_END);
  lseek(currfd,0,SEEK_SET);
  
  /* clone to memory */
  map_start=mmap(NULL,fileSize,PROT_READ,MAP_PRIVATE,currfd,0);
  if(map_start==MAP_FAILED){
    perror(input_name);
    close(currfd);
    currfd=-1;
    return;
  }   
  
  char* c=(char*) map_start;
  printf("magic number: %c%c%c\n",*(c+1),*(c+2),*(c+3));
  
  if(strncmp(c+1,"ELF",3) != 0){
    printf("Not an ELF file\n");
    return;
  }
  
  /* data encoding */
  if(*(c+5)==ELFDATANONE)
    printf("invalid encoding data\n");
  else if(*(c+5)==ELFDATA2LSB)
    printf("2's complement (little endian)\n");
  else if(*(c+5)==ELFDATA2MSB)
    printf("2's complement (big endian)\n");
  printf("entry point: %x\n",*((int*)(c+OFFSET_ENTRY)));  
  printf("section header table offset (decimal): %d\n",*((int*)(c+OFFSET_SHDT)));
  printf("Number of section headers: %d\n",*((short*)(c+OFFSET_NUMBER_OF_SHDR_ENTRIES)));
  printf("size of section header entry: %d\n",*((short*)(c+OFFSET_SIZE_OF_SECTION_HEADER_ENTRY)));
  printf("program header table offset (decimal): %d\n",*((int*)(c+OFFSET_PHDT)));
  printf("Number of program headers: %d\n",*((short*)(c+OFFSET_NUMBER_OF_PHD_ENTRIES)));
  printf("size of program header entry: %d\n",*((short*)(c+OFFSET_SIZE_OF_PROGRAM_HEADER_ENTRY)));
}

void printSectionNames(){
  if(currfd==-1){
    printf("no file descriptor exist\n");
    return;
  }
  
  // find current number of sections  e_shnum
  short numberOfSections=*((short*)(map_start+OFFSET_NUMBER_OF_SHDR_ENTRIES));

  int i; // e_shoff
  int offset=*((int*)(map_start+OFFSET_SHDT));
  offset = offset + 1;
  offset = offset - 1;
  /* get pointer to the section header table */
  Elf32_Shdr* shdrPointer=(Elf32_Shdr*)(map_start+offset);
  if(debug)
    fprintf(stderr,"pointer to the section header table: %p\n",shdrPointer);
  short shstrIndex=*((short*)(map_start+INDEX_0F_THE_SECTION_HEADER_TABLE_ENTRY_THAT_CONTAINS_THE_SECTION_NAME));
  if(debug)
    fprintf(stderr,"index of the section names: %d\n",shstrIndex);
  
  char* stringTable=(char*)(map_start+shdrPointer[shstrIndex].sh_offset);
  if(debug)
    fprintf(stderr,"string table pointer: %p\n",stringTable);
  
  for(i=0;i<numberOfSections;i++){
    if(debug)
      fprintf(stderr,"DEBUG: pointer of Elf32_Shdr[%d]: %p\n",i,shdrPointer);
    
    char* name=stringTable+shdrPointer[i].sh_name+' ';
    
    printf("[%d] %s		%x	%x	%x	%x\n",i,name ,shdrPointer[i].sh_addr, shdrPointer[i].sh_offset, shdrPointer[i].sh_size, shdrPointer[i].sh_type);
  }
}

void printSymbols(){
   if(currfd==-1){
    printf("no file descriptor exist\n");
    return;
  }
  
  
  int symtabEntry=findSymbolTableEntryInSectionHeaderTable(map_start);
  int strtabEntry=symtabEntry+1;  
  
  int SHDoffset=*((int*)(map_start+OFFSET_SHDT));
  Elf32_Shdr* shdrPointer=(Elf32_Shdr*)(map_start+SHDoffset);
  
  int size=shdrPointer[symtabEntry].sh_size;
  char* strtab=(char*)(map_start+shdrPointer[strtabEntry].sh_offset);
  Elf32_Sym* symtab=(Elf32_Sym*)(map_start+shdrPointer[symtabEntry].sh_offset);
  
  int j;
  char* section_name;
  int m=-1;

  for(j=0;j<size/sizeof(Elf32_Sym);j++){
    char* name=strtab+symtab[j].st_name;
      if (symtab[j].st_shndx == 0)
	section_name = "UND";
      else if(symtab[j].st_shndx == 65521)
	section_name = "ABS";
      else
	  m =(symtab[j].st_shndx);
    
    if(m==-1)
      printf("[%d] %x	%d	%s   %s \n",
	    j,symtab[j].st_value,symtab[j].st_shndx,name,section_name);
    else
      printf("[%d] %x	%d	%s   %d \n",
	    j,symtab[j].st_value,symtab[j].st_shndx,name,m);
      m=-1;
  }
}
int findSymbolTableEntryInSectionHeaderTable(void* map_start){
  short numberOfSections=*((short*)(map_start+OFFSET_NUMBER_OF_SHDR_ENTRIES));
  int SHDoffset=*((int*)(map_start+OFFSET_SHDT));
  int i;
  /* get pointer to the section header table */
  Elf32_Shdr* shdrPointer=(Elf32_Shdr*)(map_start+SHDoffset);
  
  for(i=0;i<numberOfSections&&shdrPointer[i].sh_type!=SHT_SYMTAB;i++){}
  return i;
}

int getSizeOfSymtab(void* map_start){
  int symtabEntry=findSymbolTableEntryInSectionHeaderTable(map_start);
  int SHDoffset=*((int*)(map_start+OFFSET_SHDT));
  Elf32_Shdr* shdrPointer=(Elf32_Shdr*)(map_start+SHDoffset);
  int size=shdrPointer[symtabEntry].sh_size;
  return size;
}

Elf32_Sym* getSymbolTable(void* map_start){
  int symtabEntry=findSymbolTableEntryInSectionHeaderTable(map_start);
  int SHDoffset=*((int*)(map_start+OFFSET_SHDT));
  Elf32_Shdr* shdrPointer=(Elf32_Shdr*)(map_start+SHDoffset);
  Elf32_Sym* symtab=(Elf32_Sym*)(map_start+shdrPointer[symtabEntry].sh_offset);
  return symtab;
}

char* getStringTableOfSymboltable(void* map_start){
  short numberOfSections=*((short*)(map_start+OFFSET_NUMBER_OF_SHDR_ENTRIES));
  int SHDoffset=*((int*)(map_start+OFFSET_SHDT));
  int i;
  /* get pointer to the section header table */
  Elf32_Shdr* shdrPointer=(Elf32_Shdr*)(map_start+SHDoffset);
  
  for(i=0;i<numberOfSections&&shdrPointer[i].sh_type!=SHT_SYMTAB;i++){}
  int symtabEntry= i;  
  int strtabEntry=symtabEntry+1;    
  return (char*)(map_start+shdrPointer[strtabEntry].sh_offset);
}
