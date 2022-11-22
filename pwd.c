#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mystring.h"
#include "pwd.h"
#define PATH_MAX 4096

int pwd(char *tokens[],int size,char * path){
    char chemin [PATH_MAX +1];
    chemin[0] = '\0';
    strcat(chemin,path);
    char buf [PATH_MAX +1];

    if (size >2){
        printf("Error in pwd , more than 1 option\n");
        return 1;
    }
    else if(size ==1){
        printf("%s",strcat(chemin,"\n"));//TODO chemin
        return 1;
    }
    else {
        if(strcmp(tokens[1],"-P")==0){
            realpath(chemin,buf);
            strcat(buf,"\n");
            printf("%s",buf);
            return 0;
        }
        else if(strcmp(tokens[1],"-L")==0){
            printf("option de pwd = -L\n");//TODO mettre la référence absolue logique 
            printf("%s",strcat(chemin,"\n"));
            return 0;
        }
        else{
            printf("Error in pwd, wrong option or not an option: use -L or -P instead.\n");
            return 1;
        }
    }
    //test branch
}
