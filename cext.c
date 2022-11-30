#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mystring.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "cext.h"
#include <dirent.h>

#define PATH_MAX 4096

int cext(char *tokens[],int size,struct string * path){
    printf("La commande %s existe\n",tokens[0]);
    return 0;
    
}