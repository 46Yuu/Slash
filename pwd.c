#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mystring.h"
#include "pwd.h"
#define PATH_MAX 4096

int pwd(char *tokens[],int size,char * path){
    char chemin [PATH_MAX +1];
    strcpy(chemin,path);
    char buf [PATH_MAX +1];

    if(size >=1){
   
        if(strcmp(tokens[1],"-P")==0){
            //realpath(chemin,buf); ne marche pas tres bien quand on a un chemin avec un lien symbolique 
            getcwd(buf,PATH_MAX);
            printf("%s \n",buf);
            return 0;
        }
        else {
        printf("%s \n",chemin);
        return 0;
        }
    }else return 1;
    
}
