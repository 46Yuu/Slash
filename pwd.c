#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mystring.h"
#include "pwd.h"
#define PATH_MAX 4096

int pwd(char *tokens[],int size,struct string * path){
    char chemin [PATH_MAX +1];
    strcat(chemin,path->data);

    if (size >2){
        printf("Error in pwd , more than 1 option\n");
        return 1;
    }
    else if(size ==1){
        strcat(chemin,"\n");
        printf("%s",chemin);//TODO chemin
        return 0;
    }
    else {
        if(strcmp(tokens[1],"-P")==0){
            printf("option de pwd = -P\n");//TODO mettre la référence absolue physique
            return 0;
        }
        else if(strcmp(tokens[1],"-L")==0){
            printf("option de pwd = -L\n");//TODO mettre la référence absolue logique 
            return 0;
        }
        else{
            printf("Error in pwd, wrong option or not an option: use -L or -P instead.\n");
            return 1;
        }
    }
}
