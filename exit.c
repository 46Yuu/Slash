#include <stdio.h>
#include <stdlib.h>
#include "exit.h"
#include <stdbool.h>

void exitVal(int val){
    exit(val);
}

void exitMain(char *tokens[],int size,int val){
    if(size ==2){
        exitVal(atoi(tokens[1]));
    }
    else if(size ==1){
        exitVal(val);
    }
    else {
        printf("Erreur");
    }
}