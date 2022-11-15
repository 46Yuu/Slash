#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mystring.h"


#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
#define MAX_FORMAT_STRLEN 30 // taille maximale pour le formatage


int main(int argc, char **argv) {
    char *chemin = "aaaaa/zzzz/eeee/ttt";
    printf(" derniere positiond de / est %s \n",strrchr(chemin,'/'));
    printf("Bah du coup la taille à enlever est %zu\n",strlen(strrchr(chemin,'/')));
    printf("---------------------Essaie avec mystring-----------------------");
    struct string * path = string_new(200);
    printf("Affichage de string vide : %s\n",path->data);
    string_append(path,"/aaa/bbb/CCC");
    printf("Affichage de apres append : %s\n",path->data);
    string_truncate_to_slash(path);
    printf("Affichage de apres truncate : %s\n",path->data);


    string_delete(path);
  



    return 0;
}