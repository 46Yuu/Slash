#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mystring.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>

#define PATH_MAX 4096


// qui test le dernier parametre si ca comment par *
 int suffix(char * monsuffix, char * machaine){
       if( strcmp(machaine+(strlen(machaine)-strlen(monsuffix)),monsuffix)==0) {return 1;}
       else {
        return 0;

    }
    }
 char ** etoile(char * args){

    // methode pour le point au debut fichier cachee
    // wd/*/*.c
    // a/b/c/*
    //a/b/c/*.c

    char * pwd = getcwd(pwd,PATH_MAX);
    char * mesfichier [] = malloc(PATH_MAX*sizeof(char *));
     //on cree un tableau de tout les arguments qui suivent la commande sans / si y'en a

    char * argument = (char*) malloc(strlen(args[1])*sizeof(char));
    strcpy(argument,args[1]);
    size_t taille = strlen(argument);
    char **tab = malloc(taille*sizeof(char));
    int i =0;
    int j= 0;
    char * strToken = strtok(argument,"/");
    while ( strToken != NULL ) {
            tab[i] =strToken;
            strToken = strtok ( NULL, "/" );
    }
    char * etoile = tab[taille-1];
    //CAS  cmd *.c
    if(etoile[0]=='*'){
        char * monsuffix = strtok(etoile,"*");
         DIR * dir =opendir(pwd);
        struct dirent *entry ; 
        if (monsuffix !=NULL){  
        while (entry = readdir(dir))
        {
           if (suffix(monsuffix,entry->d_name)==1){
            strcpy(mesfichier[j],entry->d_name);
            j++;
           }
        }
        }else { // cmd *
            while(entry=readdir(dir)){
               strcpy(mesfichier[j],entry->d_name);
                j++;
            }
        }
        
        
    }   
        
   
 }
    
   