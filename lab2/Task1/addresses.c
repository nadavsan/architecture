#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5;
int addr6;

int foo();
void point_at(void *p);
void foo1();
void foo2();

void task1c(){
    int iarray[3];
    float farray[3];
    double darray[3];
    char carray[3];
    
    /*printing iarray*/
    printf("iarray %p\n",iarray);
    printf("iarray+1 %p\n",iarray+1);
    printf("iarray+2 %p\n",iarray+2);

    /*printing farray*/
    printf("farray %p\n",farray);
    printf("farray+1 %p\n",farray+1);
    printf("farray+2 %p\n",farray+2);

    /*printing darray*/
    printf("darray %p\n",darray);
    printf("darray+1 %p\n",darray+1);
    printf("darray+2 %p\n",darray+2);

    /*printing carray*/
    printf("carray %p\n",carray);
    printf("carray+1 %p\n",carray+1);
    printf("carray+2 %p\n",carray+2);
}
void task1d(){
    int iarray[] = {1,2,3};
    char carray[] = {'a','b','c'};
    int* iarrayPtr = iarray;
    char* carrayPtr = carray;
    int* p;

    /*printing iarrayPtr*/
    printf("iarrayPtr %p\n",iarrayPtr);
    printf("iarrayPtr+1 %p\n",iarrayPtr+1);
    printf("iarrayPtr+2 %p\n",iarrayPtr+2);

    /*printing carray*/
    printf("carrayPtr %p\n",carrayPtr);
    printf("carrayPtr+1 %p\n",carrayPtr+1);
    printf("carrayPtr+2 %p\n",carrayPtr+2);

    printf("p %p\n",p);/*printing p*/

}
int main (int argc, char** argv){
    int addr2;
    int addr3;
    char* yos="ree";
    int * addr4 = (int*)(malloc(50));

    task1c();
    task1d();

    printf("- &addr2: %p\n",&addr2);
    printf("- &addr3: %p\n",&addr3);
    printf("- foo: %p\n",foo);
    printf("- &addr5: %p\n",&addr5);
    
	point_at(&addr5);
	
    printf("- &addr6: %p\n",&addr6);
    printf("- yos: %p\n",yos);
    printf("- addr4: %p\n",addr4);
    printf("- &addr4: %p\n",&addr4);
    
    printf("- &foo1: %p\n" ,&foo1);
    printf("- &foo1: %p\n" ,&foo2);
    printf("- &foo2 - &foo1: %ld\n" ,&foo2 - &foo1);
    return 0;
}

int foo(){
    return -1;
}

void point_at(void *p){
    int local;
	static int addr0 = 2;
    static int addr1;


    long dist1 = (size_t)&addr6 - (size_t)p;
    long dist2 = (size_t)&local - (size_t)p;
    long dist3 = (size_t)&foo - (size_t)p;
    
    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3);
	
	printf("- addr0: %p\n", & addr0);
    printf("- addr1: %p\n",&addr1);
}

void foo1 (){    
    printf("foo1\n"); 
}

void foo2 (){    
    printf("foo2\n");    
}
