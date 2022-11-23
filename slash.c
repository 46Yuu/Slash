#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "exit.h"
#include "pwd.h"
#include "mystring.h"
#include "cd.h"


#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
#define MAX_FORMAT_STRLEN 30 // taille maximale pour le formatage
#define MAX_TOKEN_NUMBER 10 //nombre maximale de tokens dans une ligne de commande
#define PATH_MAX 4096 // taille maximale du chemin


int main(int argc, char **argv) {
    printf("------------------------Test de Make sur slash-------------\n");


    char* input;
    char * buff;

    buff = malloc(4096*sizeof(char));

    if(buff == NULL){
        printf("buff a pas marché\n");
        return 1;
    }
    //recupération du répertoire courant et stockage dans buff
    getcwd(buff,4096);

    //Création et Remplissage de path par buff
    struct string * path = string_new(PATH_MAX);
    struct string * pathBefore = string_new(PATH_MAX);

    if(path == NULL){
        printf("path a pas marché\n");
        return 1;
    }
    if(pathBefore == NULL){
        printf("pathBefore a pas marché\n");
        return 1;
    }
    string_append(path,buff);
    string_append(pathBefore,buff);


    //Free du buff    
    free(buff);


    /* TODO: 
        -Pour le readline, faudra afficher le repertoire courant avant le $ donc faudra une variable repertoire
        courant qui changerea au fur et à mesure

        -faudra garder les valeurs de retour des return
        -Creer des dossiers test aussi pour les commandes
    */
    int val = 0;
    rl_outstream = stderr;
    char chemin[PATH_MAX];
    chemin[0] = '\0';
    /*char valeurRetour[2];
    sprintf(valeurRetour,"%d",val);
    strcat(chemin,"[");
    strcat(chemin,valeurRetour);
    strcat(chemin,"]");
    strcat(chemin,path->data);
    strcat(chemin,"$ ");*/
    tronquageA30Characteres(path->data,chemin,val);
    while ((input = readline( chemin ))) {

        int len = strlen(input);
        if (len > 0) {
            //ajoute la commande à l'historique pour l'utilisation flêches directionnelles 
            add_history(input);
        }
        //copie la commande pour pouvoir compter le nombre de cases de tokens
        char* tmp = malloc(len*sizeof(char)+1);
        if(tmp == NULL){
            free(tmp);
            exit(1);
        }
        strcpy(tmp, input);
        //compte la taille du tableau de tokens avec comme séparateur " "
        int size = 0;
        char* token = strtok(tmp, " ");
        while (token != NULL){
            size++;
            token = strtok(NULL, " ");
        }
        //crée le tableau de tokens de taille size et ajoute les tokens séparés par " " dedans
        char *tokens[size];
        int i =0;
        token = strtok(input, " ");
        while (token != NULL){
            tokens[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        if(strcmp(tokens[0],"exit") == 0){
            //printf("lancement de la fonction exit\n");
            exitMain(tokens,size,val);   
        }
        else if(strcmp(tokens[0],"cd") == 0){
            //printf("lancement de la fonction cd\n");
            val = cd(tokens,size,path);

        }
        else if(strcmp(tokens[0],"pwd") == 0){
            //printf("lancement de la fonction pwd\n");
            val = pwd(tokens,size,path->data);
        }
        else{
            //printf("Erreur , aucune commande reconnue\n");
            val = 127;
        }

        //Pour mettre la variable chemin à jour
        tronquageA30Characteres(path->data,chemin,val);

        // readline fait un malloc à chaque fois donc on dois le free à la fin
        free(tmp);
        free(input);
    }

    //Pour faire un free de path
    string_delete(path);
    return 0;
}

//Tronquage du chemin à afficher à 30 charactères
void tronquageA30Characteres(char * data, char * cheminA30Caracteres, int val){
    int len = strlen(data);
    //printf("trunc\n");
    cheminA30Caracteres[0] = '\0';
    char valeurRetour[2];
    sprintf(valeurRetour,"%d",val);
    strcat(cheminA30Caracteres,"[");
    strcat(cheminA30Caracteres,valeurRetour);
    strcat(cheminA30Caracteres,"]");
    //Si la taille est bonne, on ne touche pas au chemin; le -8 est parce qu'on devra ajouter '[0]...' et '$ ' à la fin
    if(len <= (MAX_FORMAT_STRLEN -8)){
        strcat(cheminA30Caracteres,data);
        strcat(cheminA30Caracteres,"$ ");
    }else{
        //On crée le chemin à renvoyer quand la taille dépasse 30
        strcat(cheminA30Caracteres,"...");
        strcat(cheminA30Caracteres,data+(len- (MAX_FORMAT_STRLEN -8) ));
        strcat(cheminA30Caracteres,"$ ");
    }
    //printf("trunc %s\n",data);

    
    //printf("fin trunc %s\n",cheminA30Caracteres);
    //memcpy(NvoChemin,chemin,3); //On copie les 3 premiers charactères ie "[0]" par exemple
    //strcat(NvoChemin,"..."); //On ajoute les 3 points
    //strcat(NvoChemin,chemin+(len-24)); //On colle le reste du chemin ie 24 derniers charactères

}