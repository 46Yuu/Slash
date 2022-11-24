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
    char buf [PATH_MAX +1];
    //string pwd temporaire pour garder le path d'avant les modifications avec cd , en mémoire
    char * pwd = malloc(PATH_MAX*sizeof(char));
    if(pwd == NULL){
        free(pwd);
        return 1;
    }
    strcpy(pwd,path->data);
    if (strcmp(tokens[1],"-P")==0){
        i = 2;
        goto et;
        et: 
            if (tokens[i] !=NULL){ // si il y a un chemin ou un "-" en parametre
                if (strcmp(tokens[i],"-")==0) //cd -P -
                {
                    //code
                }else{
                    DIR * dir = NULL;
                    //stock dans buf le chemin a partir de la racine
                    realpath(tokens[i],buf);
                    if((dir=opendir(buf))!=NULL){//si le dossier existe on remplace path->data par le buffer 
                        string_truncate(path,strlen(path->data));
                        string_append(path,buf);
                        closedir(dir);
                    }else{ //erreur , donc on remet le string pwd du path->data de base
                        string_truncate(path,strlen(path->data));
                        string_append(path,pwd);
                        closedir(dir);
                        return 1;
                    } 
                }         
            }else {
                //cd -P 
                string_truncate(path,strlen(path->data));
                string_append(path,getenv("HOME"));
            }
    }else{
        i = 1;
        if (strcmp(tokens[1],"-L")==0){
            i =2;
        }
        if(size >1){//si il y a des parametres après cd 
            //sépare le chemin a suivre en plusieurs token
            char tokensTmp[PATH_MAX];
            strcpy(tokensTmp,tokens[i]);
            char* token = strtok(tokensTmp, "/");
            //tant qu'il reste des string a suivre avec cd
            while (token != NULL){
                //modifie le chemin actuel par celui du cd precedant gardé en mémoire
                if (strcmp(token,"-")==0){
                    string_truncate(path,strlen(path->data));
                    string_append(path,path->dataBefore);
                }
                //recule de un répertoire
                else if (strcmp(token,"..")==0){   
                    string_truncate_to_slash(path);
                    goto suite;
                }    
                else if (strcmp(token,".")==0){
                    goto suite;
                }else{
                    //on vérifie si le répertoire existe
                    //si il existe , on ajoute a la fin du chemin actuel "/" suivi du répertoire suivant et on avance dans la liste de token
                    DIR * dir = NULL;
                    if((dir =opendir(strcat(strcat(path->data,"/"),token)))!=NULL){
                        buf[0]= '\0';
                        strcat(buf,"/");
                        strcat(buf,token);
                        string_append(path,buf);
                        closedir(dir);
                        goto suite;
                    }else{
                        closedir(dir);
                        goto et;
                    }         
                }
                suite :
                    token = strtok(NULL, "/");
            }
        }
        else {
            string_truncate(path,strlen(path->data));
            string_append(path,getenv("HOME"));
        }
    }
    //on met a jour le string correspondant au répertoire précédent
    strcpy(path->dataBefore,pwd);
    free(pwd);
    //on se déplace dans le nouveau path
    chdir(path->data);     
    return 0;
}

