#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mystring.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "cd.h"
#include <dirent.h>


#define PATH_MAX 4096

int cd(char *tokens[],int size,struct string * path){
    //les options
    int i = 0;
    char * pwd = malloc(PATH_MAX*sizeof(char));
    if(pwd == NULL){
        free(pwd);
        return 1;
    }
    strcpy(pwd,path->data);
    DIR * dir = NULL;
    char buf [PATH_MAX +1];
    if (strcmp(tokens[1],"-P")==0){
        i = 2;
        goto et;
        et: 
            if (tokens[i] !=NULL){ // si  chemin
                if (strcmp(tokens[i],"-")==0)
                {
                    /* code */
                }else{
                    realpath(tokens[i],buf);//stock dans buf le chemin a partir de la racine
                    //printf("%s\n",buf);
                    if((dir=opendir(buf))!=NULL){
                        string_truncate(path,strlen(path->data));
                        string_append(path,buf);
                        //strcpy(path->data,buf);
                    }else{
                        string_truncate(path,strlen(path->data));
                        string_append(path,pwd);
                        return 1;
                    } 
                }         
            }else {
                string_truncate(path,strlen(path->data));
                string_append(path,getenv("HOME"));
            }
    }else{
        i = 1;
        if (strcmp(tokens[1],"-L")==0)
        {
            i =2;
        }
        //printf("print 1 tokens[1]:%s\n",tokens[1]);
        if(size >1){
            char* token = strtok(tokens[i], "/");
            while (token != NULL){
                //printf("print 2 token:%s\n",token);
                if (strcmp(token,"-")==0){
                    /* code */
                    string_truncate(path,strlen(path->data));
                    string_append(path,path->dataBefore);
                }
                else if (strcmp(token,"..")==0){   
                    /* a faire */
                    string_truncate_to_slash(path);
                    goto suite;
                    //return 0;
                }    
                else if (strcmp(token,".")==0){
                    goto suite;/* a faire */
                }else{
                    if((dir =opendir(strcat(strcat(path->data,"/"),token)))!=NULL){
                        buf[0]= '\0';
                        strcat(buf,"/");
                        strcat(buf,token);
                        string_append(path,buf);
                        goto suite;
                    }else{
                        goto et;
                    }         
                }
                suite :
                    token = strtok(NULL, "/");
                    /*char tmp[PATH_MAX];        
                    getcwd(tmp,PATH_MAX);
                    printf("%s\n",tmp);*/
            }
        }
        else {
            string_truncate(path,strlen(path->data));
            string_append(path,getenv("HOME"));
        }
    }
    /*char tmp[PATH_MAX];        
    getcwd(tmp,PATH_MAX);
    printf("1 : %s\n",tmp);*/
    path->dataBefore = pwd;
    chdir(path->data);      
    /*getcwd(tmp,PATH_MAX);
    printf("2 :%s\n",tmp);*/
    //strcpy(path->data,pwd);
    closedir(dir);
    return 0;
}

