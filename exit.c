#include <stdio.h>
#include <stdlib.h>
#include "exit.h"
#include <stdbool.h>


void exitVal(int val){
    exit(val);
}

//fonction principale d'exit
void exitMain(char *tokens[],int size,int val){
    if(size ==2){//si il y a une valeur d'exit 
        exitVal(atoi(tokens[1]));
    }
    else if(size ==1){//valeur exit de default
        exitVal(val);
    }
    else {
        printf("Erreur");
    }
}