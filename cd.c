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
       et: if (tokens[2] !=NULL){ // si  chemin
            char buf [PATH_MAX +1];
            if (strcmp(tokens[2],"-")==0){
                /* code */
            }else{
                realpath(tokens[2],buf);//stock dans buf le chemin a partir de la racine
                //printf("%s\n",buf);
                if(chdir(tokens[2])==0){
                    string_truncate(path,strlen(path->data));
                    string_append(path,buf);
                    //strcpy(path->data,buf);
                    return 0;
                }else{
                    printf("Dossier inexistant\n");

                    return 1;
                } 
            }         
        }else return 1;//si aucun argument apres cd -P
    }else{
        int i = 1;
        if (strcmp(tokens[1],"-L")==0)
        {
            i =2;
        }
        
        char * pwd = malloc(PATH_MAX*sizeof(char));
        if (pwd == NULL){
            free(pwd);
            exit(1);
        }
        strcpy(pwd,path->data);
        char* token = strtok(tokens[i], "/");
        while (token != NULL){
            if (strcmp(token,"-")==0){
                /* code */
            }
            else if (strcmp(token,"..")==0){   
                /* a faire */
                string_truncate_to_slash(path);
                return 0;
            }    
            else if (strcmp(token,".")==0){
                goto suite;/* a faire */
            }
            else{
                if(chdir(token)==0){
                    strcat(pwd,"/");
                    strcat(pwd,token);
                }else{
                    goto et;
                }         
            }
            suite : token = strtok(NULL, "/");
        }
        string_truncate(path,strlen(path->data));
        string_append(path,pwd);
        //strcpy(path->data,pwd);
        return 0;
        
    }

}

