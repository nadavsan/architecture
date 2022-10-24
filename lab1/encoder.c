#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void exercise1a(int argc, char **argv){
    int c = 0;
    while ((c = fgetc(stdin)) != '\n'){
        if (c >= 65 && c <= 90)
            fprintf(stdout,"%c",'.');
        else
            fprintf(stdout,"%c",c);
    }
    fprintf(stdout,"\n");
}
void exercise1b(int argc,char **argv){
    int c = 0, upperCaseCount = 0;
    bool debugging = false;
    if (argc == 2 && strcmp(argv[1],"-D")==0)
        debugging = true;
    while ((c = fgetc(stdin)) != '\n'){
        if (c >= 65 && c <= 90){
            upperCaseCount++;
            if(debugging){                   
                fprintf(stderr, "%d", c);
                fprintf(stderr,"%c",' ');
                fprintf(stderr, "%d", '.');
                fprintf(stderr,"%c",'\n');
            }
            fprintf(stdout,"%c",'.');
        }
        else{
            if(debugging){
                fprintf(stderr, "%d", c);
                fprintf(stderr,"%c",' ');
                fprintf(stderr, "%d", c);
                fprintf(stderr,"%c",'\n');
            }   
            fprintf(stdout,"%c",c);
        }
    }
    fprintf(stdout,"%c",'\n');
    fprintf(stderr,"the number of letters: %d",upperCaseCount);
    fprintf(stdout,"%c",'\n');
}

void exercise1c(int argc,char **argv){
    bool encryptionON = false, add = false;
    int c = 0,amount = 0, amountTemp = 0, firstChar = 0,i;

    for(i = 0;i < argc; i++){
        if ((argv[i][0]=='+' || argv[i][0]=='-') && argv[i][1]=='e'){
            encryptionON = true;
            if(argv[i][0]=='+')
                add = true;
            if(argv[i][2]>=48 && argv[i][2]<=57)
                amount = (int)argv[i][2]-48;
            else if(argv[i][2]>=65 && argv[i][2]<=70)
                amount = (int)argv[i][2]-55;
            amountTemp = amount;
        }
    }
    if(encryptionON){            
        while ((c = fgetc(stdin)) != EOF){
            if(add){
                firstChar = c;

                    fprintf(stdout,"%c",firstChar);
                    while ((c = fgetc(stdin)) != '\n'){
                        fprintf(stdout,"%c",c);
                    }
                    amountTemp = amount;
                    while (amountTemp != 0){
                        fprintf(stdout,"%c",firstChar);
                        amountTemp--;
                    }
            } else{
                amountTemp = amount - 1;
                while (amountTemp > 0 && ((c = fgetc(stdin)) != '\n')){
                    amountTemp--;
                }
                while ((c = fgetc(stdin)) != '\n'){
                    fprintf(stdout,"%c",c);
                }                    
            }
            fprintf(stdout,"\n");
        }
    } else
        exercise1a(argc,argv);
}

void exercise1d(int argc,char **argv){
    bool encryptionON = false, add = false, input_from_file = false;
    int c = 0,amount = 0, amountTemp = 0, firstChar = 0,i;
    FILE *fp;

    for(i = 0;i < argc; i++){
        if ((argv[i][0]=='+' || argv[i][0]=='-') && argv[i][1]=='e'){
            encryptionON = true;
            if(argv[i][0]=='+')
                add = true;
            if(argv[i][2]>=48 && argv[i][2]<=57)
                amount = (int)argv[i][2]-48;
            else if(argv[i][2]>=65 && argv[i][2]<=70)
                amount = (int)argv[i][2]-55;
            amountTemp = amount;
        }
        if(argv[i][0]=='-' && argv[i][1] == 'i'){
            input_from_file = true;
            fp = fopen(argv[i] + 2, "r");
            if (!fp){
                fprintf(stderr,"not such file in the directory");
                exit(EXIT_FAILURE);
            }
        }
    }
    if(encryptionON){
        if(input_from_file){
            if(fp != NULL){
                while ((c = fgetc(fp)) != EOF){
                    if(add){
                        firstChar = c;

                        fprintf(stdout,"%c",firstChar);
                        while ((c = fgetc(fp)) != '\n'){
                            fprintf(stdout,"%c",c);
                        }
                        amountTemp = amount;
                        while (amountTemp != 0){
                            fprintf(stdout,"%c",firstChar);
                            amountTemp--;
                        }
                    } else{
                        amountTemp = amount - 1;
                        while (amountTemp > 0 && ((c = fgetc(fp)) != '\n')){
                            amountTemp--;
                        }
                        while ((c = fgetc(fp)) != '\n'){
                            fprintf(stdout,"%c",c);
                        }                    
                    }
                    fprintf(stdout,"\n");
                }
            }
        } else
            exercise1c(argc,argv);
    } else
        exercise1a(argc,argv);
    fclose(fp);
}

void exercise2(int argc,char **argv){
    bool encryptionON = false, add = false, input_from_file = false, output_to_file = false;
    int c = 0,amount = 0, amountTemp = 0, firstChar = 0,i;
    FILE *readf, *writef;

    for(i = 0;i < argc; i++){
        if ((argv[i][0]=='+' || argv[i][0]=='-') && argv[i][1]=='e'){
            encryptionON = true;
            if(argv[i][0]=='+')
                add = true;
            if(argv[i][2]>=48 && argv[i][2]<=57)
                amount = (int)argv[i][2]-48;
            else if(argv[i][2]>=65 && argv[i][2]<=70)
                amount = (int)argv[i][2]-55;
            amountTemp = amount;
        }
        if(argv[i][0]=='-' && argv[i][1] == 'i'){
            input_from_file = true;
            readf = fopen(argv[i] + 2, "r");
            if (!readf){
                fprintf(stderr,"not such file in the directory");
                exit(EXIT_FAILURE);
            }                
        }
        if(argv[i][0]=='-' && argv[i][1] == 'o')
            output_to_file = true;
            writef = fopen(argv[i] + 2, "w");
    }
    if(!output_to_file){
        exercise1d(argc,argv);
        fclose(readf);
    } else{
        if(encryptionON && input_from_file){
            if(readf != NULL){
                while ((c = fgetc(readf)) != EOF){
                    if(add){
                        firstChar = c;

                        fputc(firstChar,writef);
                        while ((c = fgetc(readf)) != '\n'){
                            fputc(c,writef);
                        }
                        amountTemp = amount;
                        while (amountTemp != 0){
                            fputc(firstChar,writef);
                            amountTemp--;
                        }
                    } else{
                        amountTemp = amount - 1;
                        while (amountTemp > 0 && ((c = fgetc(readf)) != '\n')){
                            amountTemp--;
                        }
                        while ((c = fgetc(readf)) != '\n'){
                            fputc(c,writef);
                        }                    
                    }
                    fputc('\n',writef);
                }
            }
            fclose(readf);
            fclose(writef);
        }else if(encryptionON){
            while ((c = fgetc(stdin)) != EOF){
                if(add){
                    firstChar = c;

                    fputc(firstChar,writef);
                    while ((c = fgetc(stdin)) != '\n'){
                        fputc(c,writef);
                    }
                    amountTemp = amount;
                    while (amountTemp != 0){
                        fputc(firstChar,writef);
                        amountTemp--;
                    }
                } else{
                    amountTemp = amount - 1;
                    while (amountTemp > 0 && ((c = fgetc(stdin)) != '\n')){
                        amountTemp--;
                    }
                    while ((c = fgetc(stdin)) != '\n'){
                        fputc(c,writef);
                    }                    
                }
                fputc('\n',writef);
            }
            fclose(writef);
        }else if(input_from_file){
            if(readf != NULL){
                while ((c = fgetc(stdin)) != '\n'){
                    if (c >= 65 && c <= 90)
                        fputc('.',writef);
                    else
                        fputc(c,writef);
                }
                fputc('\n',writef);
            }
            fclose(writef);
            fclose(readf);
        }
    }
}

int main(int argc, char **argv)
{
    /*if (argc == 1)*/
        /*exercise1a(argc,argv);*/
    /*else if (argc == 2 && strcmp(argv[1],"-D")==0)*/
        /*exercise1b(argc,argv);*/
    /*exercise1c(argc,argv);*/
    /*exercise1d(argc,argv);*/
    exercise2(argc,argv);
    return 0;
}