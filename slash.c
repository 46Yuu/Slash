#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
#define MAX_FORMAT_STRLEN 30 // taille maximale pour le formatage


int main(int argc, char **argv) {
    printf("Test de Make sur slash\n");

    char* input;
  
    /* TODO: 
        -Pour le readline, faudra afficher le repertoire courant avant le $ donc faudra une variable repertoire
        courant qui changerea au fur et à mesure

        -Faudra un fonction pour le formatage du texte de cd à 30 caractères au plus par la gauche
        -faudra garder les valeurs de retour des return
        -Creer des dossiers test aussi pour les commandes
        -Ajouter -lreadline au make file
    */
    while ((input = readline("taper $ -"))) {
        if (strlen(input) > 0) {
            //ajoute la commande à l'historique pour l'utilisation flêches directionnelles 
            add_history(input);
        }

        printf("Le truc tapé est [%s]\n", input);

        // readline fait un malloc à chaque fois donc on dois le free à la fin
        free(input);
    }

  return 0;
}