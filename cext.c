#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mystring.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "etoile.h"
#include "cext.h"
#include <dirent.h>

#define PATH_MAX 4096

int cext(char *tokens[],int size,struct string * path){
  
//  ajout etoile
    //Pour executer une commande externe avec execvp, on a besoin d'un tableau chaine de charactères dont le premier élement est la commande à lancer 
    //et le reste du tableau est les arguments sans oublier que le tableau doit se terminer par NULL
    char *args[size+1];
    for(int i = 0; i < size; i++){
        args[i] = tokens[i];
    }
    args[size] = NULL;



    int res;
    //printf("--------------------------------Tout début----------------------------\n");


    
switch (fork())
    {
        case -1:
            perror("fork");
            return 1;
        
        case 0:
        //si pas de * ou de **/    
            execvp(tokens[0],args);
            perror("exec"); //Si ca na pas fonctionné

            return 1;
        
        default:

            wait(&res);
            if(WEXITSTATUS(res) == 0)
                return 0; 
            return WEXITSTATUS(res);
            //write(STDOUT_FILENO,"exec ok",8);
            //printf("................Execution terminée avec le code d'erreur %d \n",WEXITSTATUS(res));
            //exit(WEXITSTATUS(res));
            
    }
    return 0;
       
}
    
