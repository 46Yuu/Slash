#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>




#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
#define MAX_FORMAT_STRLEN 30 // taille maximale pour le formatage
#define PATH_MAX 4096


int main(int argc, char **argv) {
    char * buff = malloc(4096*sizeof(char));
    getcwd(buff,4096);
    printf("%s\n",buff);
    printf("La taille du truc est %zu\n",strlen(buff));
    free(buff);
    char toto[100];
    strcat(toto,"Bien");
    printf("toto est  %s\n",toto);
    toto[0] = '\0';
    printf("Now toto est  %s\n",toto);
    strcat(toto,"Bien");
    printf("Enfin toto est  %s\n",toto);




    return 0;
}