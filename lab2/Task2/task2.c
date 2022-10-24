#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
char censor(char c);
char encrypt(char c);
char decrypt(char c);
char dprt(char c);
char cprt(char c);
char my_get(char c);
char quit(char c);
char toLower(char c);
char* map(char *array, int array_length, char (*f) (char));
void menu();

struct fun_desc {
  char *name;
  char (*fun)(char);
};


/* Gets a char c and returns its encrypted form by adding 2 to its value. 
            If c is not between 0x41 and 0x7a it is returned unchanged */
char encrypt(char c){
  char ans = c;
  if(c >= 65 && c <= 122)
    ans = c + 2;
  return ans;
}

/* Gets a char c and returns its decrypted form by reducing 2 to its value. 
            If c is not between 0x41 and 0x7a it is returned unchanged */
char decrypt(char c){
  char ans = c;
  if(c >= 65 && c <= 122)
    ans = c - 2;
  return ans;
}

/* dprt prints the value of c in a decimal representation followed by a 
           new line, and returns c unchanged. */
char dprt(char c){
  fprintf(stdout,"%d\n",(int)c);
  return c;
}

/* If c is a number between 0x41 and 0x7a, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('*') character. After printing, cprt returns 
                    the value of c unchanged. */
char cprt(char c){
  if(c >= 65 && c <= 122)
    fprintf(stdout,"%c\n",c);
  else
    fprintf(stdout,"%c\n",'*');
  return c;
}


char my_get(char c){/* Ignores c, reads and returns a character from stdin using fgetc. */
  return fgetc(stdin);
}

char censor(char c) {
  if(c == '!')
    return '*';
  else
    return c;
}
char toLower(char c){
  if(c>=65&&c<=90)
    return c+('A'-'a');
  return c;
}
 
char* map(char *array, int array_length, char (*f) (char)){
  int i = 0;
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for (i = 0; i < array_length; i++){
    mapped_array[i] = f(array[i]);
  }
  return mapped_array;
}
/* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
char quit(char c){
  if(c == 'q')
    exit(0);
  return c;
}




void menu_func(){
  struct fun_desc menu[] = { { "Censor", censor }, { "Encrypt", encrypt },{ "Decrypt", decrypt }, { "Print dec", dprt },{ "Print string", cprt }, {"Get string", my_get}, {"Quit",quit},{ NULL, NULL } };
  char* carray = malloc(5*sizeof(char));//FREE!!!
  char buf[10];
  int i = 0,func_num=0;

  while(1==1){
    fprintf(stdout,"Please choose a function:\n");
    while(menu[i].name !=NULL){
      fprintf(stdout,"%d) %s\n",i,menu[i].name);
      i++;
    }
    fprintf(stdout,"Option: ");
    fgets(buf,10,stdin);
    sscanf(buf,"%d",&func_num);

    if(func_num>=0&&func_num<i){
      fprintf(stdout,"Within bounds\n");
      if(strcmp(menu[func_num].name,"Quit")==0)
        return 0;
      char* carray_old = carray;
      carray = map(carray, 5, menu[func_num].fun);
      free(carray_old);
    }
    else{
      fprintf(stdout,"Not within bounds\n");
      return 0;
    }
    i=0;
  }
}
int main(int argc, char **argv){
  menu_func();
 return 0;
}
  /*char arr1[] = {'H','e','y','!'};
  char* arr2 = map(arr1, 4, toLower);
  printf("%s\n", arr2); 
  free(arr2);*/

  /*int base_len = 5;
  char arr1[base_len];
  char* arr2 = map(arr1, base_len, my_get);
  char* arr7 = map(arr2, base_len, quit);
  char* arr3 = map(arr2, base_len, encrypt);
  char* arr4 = map(arr3, base_len, dprt);
  char* arr5 = map(arr4, base_len, decrypt);
  char* arr6 = map(arr5, base_len, cprt);
  
  free(arr2);
  free(arr3);
  free(arr4);
  free(arr5);
  free(arr6);
  free(arr7);
  return 0;
}*/