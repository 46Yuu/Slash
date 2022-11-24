#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mystring.h"
#include "pwd.h"
#define PATH_MAX 4096

int pwd(char *tokens[],int size,char * path){
    //chemin permettra de garder une copie du path initial ou du path final
    char chemin [PATH_MAX +1];
    strcpy(chemin,path);
    //buf sert de buffer pour faire des concatenations de string avant leurs ajout dans chemin
    char buf [PATH_MAX +1];
    //si la commande contient plus d'arguments que 1 
    if (size >1){
        if(strcmp(tokens[1],"-P")==0){
            //on mets dans le buf le string correspondant au path de la racine jusqu'au répertoire courant
            getcwd(buf,PATH_MAX);
            //si la commande contient plus d'arguments que 2
            //on ajoute les arguments a la fin de buf avec un '/' avant chacun d'entre eux
            if(size >2){
                strcat(buf,"/");
                strcat(buf,tokens[2]);
            }
            //on récupère a le path absolue du path de buf et on le met dans chemin
            realpath(buf,chemin);
            printf("%s\n",chemin);
            return 0;
        }
        if(strcmp(tokens[1],"-L")==0){
            //si la commande contient plus d'arguments que 2
            //on ajoute les arguments a la fin de chemin avec un '/' avant chacun d'entre eux
            if(size >2){
                strcat(chemin,"/");
                strcat(chemin,tokens[2]);
            }
            printf("%s\n",chemin);
            return 0;
        }
    }
    else {//pwd sans arguments
        printf("%s\n",chemin);
        return 0;
    }
    return 0;
}
