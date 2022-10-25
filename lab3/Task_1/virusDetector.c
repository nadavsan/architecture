#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};
struct fun_desc {
  char *name;
  char (*fun)(char);
};

FILE* input;
FILE* output;

void readVirus(virus* vir, FILE* input){
  fprintf(stdout,"entered read virus\n");
  //read size
  int read = fread(&vir->SigSize,sizeof(unsigned short),sizeof(char),input);
  fprintf(stdout,"size was read = %d\nread = %d\n",vir->SigSize,read);
  if((read < 1)){// || vir->SigSize <= 0){
    fprintf(stdout,"inside if: size was read = %d\n",vir->SigSize);
    vir = NULL;
    return;
  }
  // read sig
  size_t tmp = vir->SigSize;
  vir->sig = (unsigned char *) calloc(tmp,sizeof(unsigned char));//don't forget to FREE!!!!!
  fprintf(stdout,"created space on the heap for sig\n");
  read = fread(vir->sig,vir->SigSize,sizeof(unsigned char),input);
  if(read < 1){
    vir = NULL;
    return;
  }
  fprintf(stdout,"sig was read");
  //read name
  read = fread(vir->virusName,16,1,input);
  if(read < 1){
    vir = NULL;
    return;
  }
  fprintf(stdout,"name was read = %s\n", vir->virusName);
}
void printVirus(virus* vir, FILE* output){
  fprintf(stdout,"entered print virus\n");
  int i;
  fprintf(output, "Virus name: %s\nVirus size: %i\nSignature:",vir->virusName,vir->SigSize);
  for( i = 0; i < vir->SigSize; i++){
    if(i % 20 == 0){
      fprintf(output,"\n");
    }
    fprintf(output,"%02X ", vir->sig[i]);
  }
  fprintf(output,"\n\n");
}

/* Print the data of every link in list to the given stream. Each item followed by a newline character. */
void list_print(link *virus_list, FILE* output){
  virus *vir;
  link* temp_list = virus_list;
  fprintf(stdout,"in list_print\n");
  while(temp_list != NULL){
    vir = temp_list->vir;
    printVirus(vir,output);
    temp_list = temp_list->nextVirus;
  }
}

/* Add the given link to the list 
        (either at the end or the beginning, depending on what your TA tells you),
        and return a pointer to the list (i.e., the first link in the list).
        If the list is null - return the given entry. */
link* list_append(link* virus_list, link* to_add){
  to_add->nextVirus = virus_list;
  return to_add;
}
link* load_signatures(link* virus_list,virus* virus){
  FILE *input; 
  char name[200];
  char rawInput[200];
  bool firstRound = true;
  // insert name
  fgets(rawInput,sizeof(rawInput),stdin);
  sscanf(rawInput,"%s",name);

  input = fopen(name,"r");
  while (!feof(input)){
    fprintf(stdout,"entered while loop in load_signatures\n");
    virus = calloc(1,sizeof(virus));
    readVirus(virus,input);
    
    fprintf(stdout,"line 108 %d\n",*virus);
    if(virus->sig != NULL){
      fprintf(stdout,"printing\n");
      printVirus(virus,stdout);
      if(firstRound){
        firstRound = false;
        virus_list->vir = virus;
        fprintf(stdout,"virus_list->vir->name = %s\n",virus_list->vir->virusName);
        virus_list->nextVirus = NULL;
      }
      else {
        link* to_add = calloc(1,sizeof(link));
        to_add->vir = virus;
        virus_list=list_append(virus_list,to_add);
        fprintf(stdout,"virus_list->vir->name = %s\n",virus_list->vir->virusName);
      }
    }
    else{
      fprintf(stdout,"free virus and exit while loop\n");
      break;
    
    }
  } 
    fprintf(stdout, "here2\n");
    return virus_list;
}


/* Free the memory allocated by the list. */
void list_free(link *virus_list){
  int i = 1;
  while(virus_list != NULL){
    fprintf(stdout,"%s_%d\n",virus_list->vir->virusName,i++);
    link *next = virus_list;
    fprintf(stdout,"next initialized\n");
    free(virus_list->vir->sig);
    fprintf(stdout,"free(virus_list->vir->sig);\n");
    free(virus_list->vir);
    fprintf(stdout,"free(virus_list->vir);\n");
    virus_list = virus_list->nextVirus;
    free(next);
    fprintf(stdout,"free(next);\n");
  }
}
void print_signatures(link *virus_list){
  fprintf(stdout,"entered print_signatures\n");
  list_print(virus_list,stdout);
}
void menu_func(){
   struct fun_desc menu[] = { { "Load signatures", load_signatures}, { "Print signatures", print_signatures },{"Quit",NULL},{ NULL, NULL } };
  char buf[200];
  int i = 0,func_num=0;
  link* virus_list = calloc(1,sizeof(link));
  virus* virus;// = malloc(sizeof(virus));
  while(1){
    fprintf(stdout,"Please choose a function:\n");
    while(menu[i].name !=NULL){
      fprintf(stdout,"%d) %s\n",i,menu[i].name);
      i++;
    }
    fprintf(stdout,"Option: ");
    fgets(buf,sizeof(buf),stdin);
    sscanf(buf,"%d",&func_num);
    if(func_num>=0&&func_num<i){
      fprintf(stdout,"Within bounds\n");
      if(func_num == 2){//strcmp(menu[func_num].name,"Quit")==0){
        fprintf(stdout,"list_free was called form menu_func\n");
        list_free(virus_list);
        return;
      }
      else if(func_num == 0)//strcmp(menu[func_num].name,"Load signatures")==0)
        virus_list = load_signatures(virus_list,virus);
      else if(func_num == 1)//strcmp(menu[func_num].name,"Print signatures")==0)
        print_signatures(virus_list);
    }
    else{
      fprintf(stdout,"Not within bounds\n");
      return;
    }
    i=0;
  }
}

int main(int argc,char *argv[]) {
  menu_func();
  /*// FILE *fp=NULL; 
  // char* name = argv[1];
  // fprintf(stdout,"name=%s\n",name);
  // fp = fopen(name,"r");
  // virus* vir =(virus *) calloc (1, sizeof(virus));
  // readVirus(vir,fp);
  // printVirus(vir,stdout);
  // free(vir->sig);
  // free(vir);*/


  // /* menu_func();*/
  // fclose(fp);
   return 0;

  // virus *vir = malloc(sizeof(virus));
  // readVirus(vir,fp);
}