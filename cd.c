#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mystring.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "cd.h"

#define PATH_MAX 4096

int cd(char *tokens[],struct string * path){
 

    //les options
    if (strcmp(tokens[1],"-P")==0){
       et: if (tokens[2] !=NULL){
            char buf [PATH_MAX +1];
            realpath(tokens[2],buf);
                if(chdir(tokens[2])==0){
                    strcpy(path->data,buf);
                    return 0;
                }else{
                    return 1;
                }
            return 1; //si aucun argument apres cd -P
        }else return 1;

    }else{
        int i = 2;
        char * pwd = malloc(PATH_MAX*sizeof(char));
        strcpy(pwd,path->data);
       
       char* token = strtok(tokens[2], "/");
        while (token != NULL){
            if(chdir(token)==0){
                strcat(pwd,"/");
                strcat(pwd,token);
 
            }else{
                goto et;
            }
            token = strtok(NULL, "/");
        }
        strcpy(path->data,pwd);
        return 0;
        
    };
   

}

