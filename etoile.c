#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "mystring.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include "etoile.h"

#define PATH_MAX 4096


// qui test le dernier parametre si ca commence par *
 int suffix(char * monsuffix, char * machaine){
       if( strcmp(machaine+(strlen(machaine)-strlen(monsuffix)),monsuffix)==0) {return 1;}
       else {
        return 0;

    }
    }
char ** etoile(char ** args, int size, struct string * chemin,char **argv,int * nb_argv){

    //variables
    char * nom = malloc ((char *)sizeof(char));  
    int k;
    char * pwd = malloc(PATH_MAX*sizeof(char));
    memset(pwd,0,sizeof(char)*PATH_MAX);
    char * path = malloc(PATH_MAX*sizeof(char));
    memset(path,0,sizeof(char)*PATH_MAX);
    DIR * dir = NULL;
    struct dirent * entry = NULL;
    struct  stat st;

    // CAS 1  cmd sans chemin
    
    dir=opendir(chemin);
    
    if (dir == NULL){ goto error;}
    if (size == 1)  //CAS  cmd *.c \ cmd * sur le rep courant 
    {
        while ((entry=readdir(dir)))
        {
          sprintf(path,"%s/%s",chemin,entry->d_name);

        if(stat(path,&st)==-1){goto error;}
        if(strcmp(args[k],"*")==0){
             nom = entry->d_name;
            if ((strcmp(nom[0],"." )!=0) && (suffix(pwd,nom)==1)){
                argv[*nb_argv]=malloc(sizeof(char)*PATH_MAX);
                sprintf(argv[*nb_argv],"%s",path);
                (*nb_argv)++;
            }
        }else{

            if((strcmp(args[k],entry->d_name)==0) && (suffix(".",entry->d_name[0])==1)){
                argv[*nb_argv]=malloc(sizeof(char)*PATH_MAX);
                sprintf(argv[*nb_argv],"%s",path);
                (*nb_argv)++; 
            }
        }
        
    }
    }
    closedir(dir);
    free(path);
    free(pwd);
    return  argv;  
    error : {
    perror("stat");
    free(pwd); 
    free(path);
    return NULL; 
    }
      
   
 }
    


   