#include <stdio.h>
#define	MAX_LEN 34			/* maximal input string size */
					/* enough to get 32-bit string + '\n' + null terminator */
extern void assFunc(int x);

char c_checkValidity(int x){
    fprintf(stdout,"check validity x=%d\n",x);
    if(x>=0)
        return '1';
    return '0';
}

int main(int argc, char** argv)
{
    char buf[MAX_LEN];
    int x = 0;
    fgets(buf, MAX_LEN, stdin);
    sscanf(buf, "%d", &x);
    assFunc(x);
    return 0;
}