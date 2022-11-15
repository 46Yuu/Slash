#include <stdlib.h>
#include <string.h>
#include "mystring.h"


// Crée une nouvelle chaîne de longueur 0 et de capacité donnée, allouée
// sur le tas (c'est-à-dire avec malloc).
struct string * string_new(size_t capacity){
    struct string *s;
    s = malloc(sizeof(struct string));
    if(s == NULL){
        free(s);
        return NULL;
    } 
    s->capacity = capacity;
    s->length = 0;

    char * data = malloc(capacity * sizeof(char));
    if(data == NULL){
        free(data);
        return NULL;
    } 
    s->data = data;
    return s;

}

// Détruit une chaîne, en libérant la mémoire occupée.
void string_delete(struct string * str){
    free(str->data);
    free(str);
}

// Met à jour dest en ajoutant src à la *fin*.
// Renvoie 1 en cas de réussite, 0 en cas d'échec.
// Échoue sans rien modifier si le buffer n'est pas assez grand.
int string_append (struct string * dest, char * src){

    //length ne compte pas le caractere vide
    if((dest->length + strlen(src) +1 ) > dest->capacity)
        return 0;

    memmove(dest->data+dest->length, src,strlen(src)+1);
    dest->length = dest->length + strlen(src);
    return 1;
}

// Tronque la chaîne en enlevant les nchars derniers caractères
// (ou tous les caractères s'il y en a moins de nchars).
void string_truncate (struct string * str, size_t nchars){
    if(nchars >= str->length)
    {
        str->length = 0;
        str->data[0] = '\0';
    }
    else{
        //PAs de +1 car ca commence à 0
        str->length = str->length - nchars;
        str->data[str->length] = '\0';
    }
}


//Tronque la chaine de la fin vers le debut jusqu'a trouver le premier "/" pour gérer les chemins (les cd ..)
void string_truncate_to_slash (struct string * str){
    
    string_truncate(str,strlen(strrchr(str->data,'/')));

}

