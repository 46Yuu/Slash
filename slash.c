#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "exit.h"
#include "pwd.h"
#include "mystring.h"
#include "cd.h"
#include "etoile.h"
#include "cext.h"
#include <stdbool.h>
#include <dirent.h>
#include <signal.h>


#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
#define MAX_FORMAT_STRLEN 30 // taille maximale pour le formatage
#define MAX_TOKEN_NUMBER 10 //nombre maximale de tokens dans une ligne de commande
#define PATH_MAX 4096 // taille maximale du chemin
#define PATH_100 100 // taille maximale du chemin
#define MAX_TAILLE_NOM_FICHIER  300
#define MAX_TOKEN_ETOILE 100 //Nombre max de fichier récuperable avec les étoiles
#define PATH_COMMANDE_EXTERNE "/usr/bin"

int val;
//Tronquage du chemin à afficher à 30 charactères
void tronquageA30Characteres(char * data, char * cheminA30Caracteres, int val, int taille_val){
    int len = strlen(data);
    //printf("trunc\n");
    cheminA30Caracteres[0] = '\0';
    char valeurRetour[taille_val+1];
    sprintf(valeurRetour,"%d",val);
    strcat(cheminA30Caracteres,"[");
    if(val == 255){
        strcat(cheminA30Caracteres,"\001\033[32m\002");
        strcat(cheminA30Caracteres,"SIG");
        strcat(cheminA30Caracteres,"\001\033[00m\002");
        strcat(cheminA30Caracteres,"]");
    }
    else if(val == 0){
        strcat(cheminA30Caracteres,"\001\033[32m\002");
    }
    else {
        strcat(cheminA30Caracteres,"\001\033[91m\002");
    }
    strcat(cheminA30Caracteres,valeurRetour);
    strcat(cheminA30Caracteres,"\001\033[00m\002");
    strcat(cheminA30Caracteres,"]");
    //Si la taille est bonne, on ne touche pas au chemin; le -7 est parce qu'on devra ajouter '[0]...' et '$ ' à la fin
    //du coup il y'aura taille_val aussi d'enlevé
    if(len <= (MAX_FORMAT_STRLEN -7-taille_val)){
        strcat(cheminA30Caracteres,data);
        strcat(cheminA30Caracteres,"$ ");
    }else{
        //On crée le chemin à renvoyer quand la taille dépasse 30
        strcat(cheminA30Caracteres,"...");
        strcat(cheminA30Caracteres,data+(len- (MAX_FORMAT_STRLEN -7-taille_val) ));
        strcat(cheminA30Caracteres,"$ ");
    }
    //printf("trunc %s\n",data);
    //printf("fin trunc %s\n",cheminA30Caracteres);
    //memcpy(NvoChemin,chemin,3); //On copie les 3 premiers charactères ie "[0]" par exemple
    //strcat(NvoChemin,"..."); //On ajoute les 3 points
    //strcat(NvoChemin,chemin+(len-24)); //On colle le reste du chemin ie 24 derniers charactères
}

//Verifie si la commande externe existe c'est à dire si elle est dans le fichier /usr/bin
int existenceCommandeExterne(char * cmd){
    int retour = 0;
    //printf("La commande est %s",cmd);
    DIR * dir = opendir(PATH_COMMANDE_EXTERNE);
    if(dir == NULL){
        return 0;
    }
    struct dirent * entree = NULL;
    int compteur = 0;
    while((entree = readdir(dir))){
        //printf("Compteur est %d",compteur);
        compteur++;
        //printf("%s ",entree->d_name);
        if(strcmp(entree->d_name,cmd) == 0){
            //printf("Trouvé");
            retour = 1;
            break;
        }
    }
    closedir(dir);
    return retour;
}

// separe la chaine selon le separateur
char ** tokage(char * chaineASeparer,char separateur,int * taille){
    char  tmpSeparateur[1]="";
    tmpSeparateur[0]=separateur;
    int len = strlen(chaineASeparer);
    //copie la commande pour pouvoir compter le nombre de cases de tokens
        char* tmp = malloc(len*sizeof(char)+1);
        if(tmp == NULL){
            free(tmp);
            exit(1);
        }
        strcpy(tmp, chaineASeparer);
        //compte la taille du tableau de tokens avec comme séparateur " "
        int size = 0;
        char* token = strtok(tmp,tmpSeparateur);
        while (token != NULL){
            size++;
            token = strtok(NULL,tmpSeparateur);
        }
        //crée le tableau de tokens de taille size et ajoute les tokens séparés par " " dedans
        char**tokens = malloc(size*sizeof(char*)+1);
        if(tokens == NULL){
            free(tokens);
            exit(1);
        }
        int i =0;
        token = strtok(chaineASeparer, tmpSeparateur);
        while (token != NULL){
            tokens[i] =malloc(strlen(token)*sizeof(char)+1);
            if( tokens[i]== NULL){
                free(tokens[i]);
                exit(1);
            }
            strcpy(tokens[i],token);
            //tokens[i]    = token;
            i++;
            token = strtok(NULL, tmpSeparateur);
        }  
        free(tmp);
        *taille = size;
        //printf("juste ici !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return tokens;
}

//On verfie si le chemin donné est correct avant d'executer la commande
int existenceCheminVersCmdExt(char * chemin,char *cmd){
    //printf("Arrivé ici au moins et le dossier est %s et cmd est %s\n",chemin,cmd);
    int retour = 0;
    //printf("La commande est %s",cmd);
    DIR * dir = opendir(chemin);
    if(dir == NULL){
        //printf("Overture non réussi\n");
        return -1;
    }
    //printf("Overture réussi\n"result);
    struct dirent * entree = NULL;
    while((entree = readdir(dir))){
        //printf("%s ",entree->d_name);
        if(strcmp(entree->d_name,cmd) == 0){
            //printf("Trouvé\n");
            retour = 1;
            break;
        }
    }
    closedir(dir);
    return retour;
}

//On verfie si on a le droit d'executer la commande
int droitdExecuterLaCmdExt(char * chemin,char *cmd){
    //printf("Arrivé ici au moins et le dossier est %s et cmd est %s\n",chemin,cmd);
    int retour = 0;
    //printf("La commande est %s",cmd);
    DIR * dir = opendir(chemin);
    struct stat st;
    if(dir == NULL){
        //printf("Overture non réussi\n");
        return -1;
    }
    //printf("Overture réussi\n");
    struct dirent * entree = NULL;
    while((entree = readdir(dir))){
        //printf("%s ",entree->d_name);
        if(strcmp(entree->d_name,cmd) == 0){
            //printf("Trouvé\n");
            //printf("************");
            char chemin_vers_fichier[100] = "";
            strcpy(chemin_vers_fichier,chemin);
            strcat(chemin_vers_fichier,"/");
            strcat(chemin_vers_fichier,cmd);
            if(stat(chemin_vers_fichier,&st) == -1){
                retour = 0;
                break;
            }
            if(((st.st_mode & S_IXUSR) == S_IXUSR) || ((st.st_mode & S_IXGRP) == S_IXGRP) || ((st.st_mode & S_IXOTH) == S_IXOTH)) {
                retour = 1;
            }
            break;
        }
    }
    closedir(dir);
    return retour;
}

int main(int argc, char **argv) { 
    val = 0;
    char* input;
    char * buff;
    buff = malloc(PATH_100*sizeof(char));
    if(buff == NULL){
        printf("buff a pas marché\n");
        return 1;
    }
    //recupération du répertoire courant et stockage dans buff
    getcwd(buff,PATH_100);
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
    tronquageA30Characteres(path->data,chemin,val,1);
    signal(SIGINT,SIG_IGN);
    signal(SIGTERM,SIG_IGN);
    while ((input = readline( chemin ))) {
        int len = strlen(input);
        if (len > 0) {
            //ajoute la commande à l'historique pour l'utilisation flêches directionnelles 
            add_history(input);
            
        }else{
            continue;
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
            //Cas des commandes externes
            //On verfie au debut si il y'a étoiles dans les arguments et si oui
            //On cree un nouveau tableau token avec les anciens arguments de token
            //puis la partie où il y avait étoile remplacé par les fichiers trouvé
            int nb_arg_tokens_avec_fichiers_etoile = 0;
            char **tokens_avec_fichiers_etoile = malloc(sizeof(char*[PATH_100]));
            int il_ya_eu_etoile = 0;
            int compteur_etoile = 0;
            if (size >1){
                for (int i=0;i< size;i++){
                    if(strstr(tokens[i],"*")){ 
                        //le rep courant
                        char * repCourant = malloc(100*sizeof(char));
                        getcwd(repCourant,100);
                         //Pour verfiier si le chemin commence a la racine et 
                         // contine le chemin du rep courant dedans
                         // si c'est le cas on le supprime de la chaine tokens[i]
                        int commenceParSlash = 0;
                        if(tokens[i][0] == '/'){
                            commenceParSlash = 1;
                        }
                        if(strstr(tokens[i],repCourant)){
                            strcpy(tokens[i],tokens[i]+strlen(repCourant)+1);
                        }
                        nb_arg_tokens_avec_fichiers_etoile = size;
                        compteur_etoile ++;
                        char * repEtoile =malloc(PATH_100*sizeof(char));
                        memset(repEtoile,0,PATH_100*sizeof(char));
                        int nb_argv = 0;
                        int *p_nb_argv = &nb_argv;
                        char **argv = malloc(sizeof(char*[MAX_TOKEN_ETOILE]));
                        if(argv == NULL){
                            printf("Malloc a pas marché\n");
                            free(argv);
                            return val;                        
                        }
                        int t = 0;
                        int * taillePatherne = &t;
                        char ** patherne = tokage(tokens[i],'/',taillePatherne); 
                        int result = etoile(patherne,taillePatherne,repEtoile,argv,p_nb_argv,commenceParSlash,repCourant);
                        //On copie jusqu'a i les élements de tokens
                        if (compteur_etoile ==1){ //on recopie cette partie que la premiere fois
                            for(int j = 0;j < i;j++){
                                tokens_avec_fichiers_etoile[j]=malloc(MAX_TAILLE_NOM_FICHIER * sizeof(char)+1); 
                                if(tokens_avec_fichiers_etoile[j] == NULL){
                                    printf("Malloc a pas marché\n");
                                    free(tokens_avec_fichiers_etoile[j]);
                                    return 0;                        
                                }
                                sprintf(tokens_avec_fichiers_etoile[j],"%s",tokens[j]);
                                //strcpy(tokens_avec_fichiers_etoile[j],tokens[j]);
                                //tokens_avec_fichiers_etoile[j] = tokens[j];
                            }
                        }
                        result++;
                        //On copie ensuite tous les éléments du tableau argv
                        for(int j = i,k = 0;j< i+nb_argv;j++,k++){
                            tokens_avec_fichiers_etoile[j]=malloc(MAX_TAILLE_NOM_FICHIER * sizeof(char)+1); 
                            if(tokens_avec_fichiers_etoile[j] == NULL){
                                printf("Malloc a pas marché\n");
                                free(tokens_avec_fichiers_etoile[j]);
                                return 0;                        
                            }
                            sprintf(tokens_avec_fichiers_etoile[j],"%s",argv[k]);
                        }
                        //Enfin on copie le reste de ce qu'il yavait dans tokens a partir de i
                        for(int j = i+nb_argv,k = i+1;k < size;j++,k++){
                            tokens_avec_fichiers_etoile[j]=malloc(MAX_TAILLE_NOM_FICHIER * sizeof(char)+1); 
                            if(tokens_avec_fichiers_etoile[j] == NULL){
                                printf("Malloc a pas marché\n");
                                free(tokens_avec_fichiers_etoile[j]);
                                return 0;                        
                            }
                            sprintf(tokens_avec_fichiers_etoile[j],"%s",tokens[k]);
                        }
                        /*printf("Fichers etoile avant %d\n",nb_arg_tokens_avec_fichiers_etoile);
                        for(int i=0;i< nb_arg_tokens_avec_fichiers_etoile;i++){
                        printf("%s\n",tokens_avec_fichiers_etoile[i]);
                        }*/
                        //On met à jour la taille du tableau contenant la commande et les fichiers etoiles
                        nb_arg_tokens_avec_fichiers_etoile = nb_arg_tokens_avec_fichiers_etoile + nb_argv;
                        il_ya_eu_etoile = compteur_etoile;
                        //printf("Fichers etoile %d\n",nb_arg_tokens_avec_fichiers_etoile);
                        //On libére le tableau argv et les autres pointeur qu'on utilisera plus
                        for(int i = 0; i < nb_argv; i++){
                            free(argv[i]);
                        }
                        // printf("t = %d\n",t);
                        // printf("taillePatherne = %d\n",*taillePatherne);
                        for(int i = 0; i <t;i++){
                            // printf("patherne[i] = %s\n",patherne[i]);
                            free(patherne[i]);
                        }
                        free(argv);
                        free(patherne);
                        free(repEtoile);
                        free(repCourant);
                        //break;
                    }
                }
            }
            /*printf("Fichers etoile %d\n",nb_arg_tokens_avec_fichiers_etoile);
            for(int i=0;i< nb_arg_tokens_avec_fichiers_etoile;i++){
            printf("%s\n",tokens_avec_fichiers_etoile[i]);
            }*/
            nb_arg_tokens_avec_fichiers_etoile = nb_arg_tokens_avec_fichiers_etoile -1;
            //On verifier si c'est un chemin vers une commande externe
            int estCheminVersCmdExt = 0;
            for(int i = 0; i < strlen(tokens[0]); i++){
                if(tokens[0][i] == '/'){
                    //rintf("%c trouvé à la position %d\n",tokens[0][i],i);
                    estCheminVersCmdExt = 1;
                    break;
                }
            }
            //Le cas où c'est un chemin vers une commande externe
            if(estCheminVersCmdExt == 1){
                //compte la taille du tableau de tokens[0] avec comme séparateur "/"
                int tailleApresSeparation = 0;
                char * tmpToken0 = malloc(strlen(tokens[0])*sizeof(char)+1);
                char * save = malloc(strlen(tokens[0])*sizeof(char)+1);
                char * savePourSecondTok = malloc(strlen(tokens[0])*sizeof(char)+1);
                if(tmpToken0 == NULL){
                    free(tmpToken0);
                    exit(1);
                }
                if(save == NULL){
                    free(save);
                    exit(1);
                }
                if(savePourSecondTok == NULL){
                    free(savePourSecondTok );
                    exit(1);
                }
                strcpy(tmpToken0,tokens[0]);
                strcpy(save,tokens[0]);
                strcpy(savePourSecondTok,tokens[0]);
                //printf("tokens  0 est %s\n",tmpToken0);
                char* tok = strtok(tmpToken0, "/");
                while (tok != NULL){
                    tailleApresSeparation++;
                    tok = strtok(NULL, "/");
                }
                //crée le tableau de toutes les parties du chemin de taille tailleApresSeparation 
                //et ajoute les parties séparés par "/" dedans
                char *tokens0[tailleApresSeparation];
                int i =0;
                char * tokk = strtok(savePourSecondTok, "/");
                while (tokk != NULL){
                    tokens0[i] = tokk;
                    i++;
                    tokk = strtok(NULL, "/");
                }
                //On enleve la commande a la fin du chemin et on obtient
                //le chemin vers le dossier où est stocké la commande
                //tailleApresSeparation - 1 est la fin du tableau c'est-à-dire là où on a la commande
                int tailleAEnlever = strlen(tokens0[tailleApresSeparation - 1]);
                //printf("La taille à enlever est donc %d\n",tailleAEnlever);
                save[strlen(save) - tailleAEnlever] = '\0';
                //printf("Enfin save est %s\n",save);
                //Quand le chemin vers la commande est correcte alors on lance la commande
                if(existenceCheminVersCmdExt(save,tokens0[tailleApresSeparation - 1]) == 1){
                    if(droitdExecuterLaCmdExt(save, tokens0[tailleApresSeparation - 1])==1){
                        tokens[0] = tokens0[tailleApresSeparation - 1];
                        //printf("Bref tokens[0] est %s \n",tokens[0]);
                        if (il_ya_eu_etoile) val = cext(tokens_avec_fichiers_etoile,nb_arg_tokens_avec_fichiers_etoile,path); 
                        else val = cext(tokens,size,path);
                    }
                    else{
                        val = 126;
                        //write(STDERR_FILENO,"bash: ./nonexistent: No such file or directory\n",50);
                        write(STDERR_FILENO,"bash: ",7);
                        write(STDERR_FILENO,tokens[0],strlen(tokens[0]));
                        write(STDERR_FILENO,": Permission denied\n",21);
                    }
                }
                else{//Le cas où le chemin est incorrecte on renvoie la valeur d'erreur 1
                    //tokens[0] = tokens0[tailleApresSeparation - 1];
                    //printf("Bref tokens[0] est %s \n",tokens[0]);
                    // if (il_ya_eu_etoile) val = cext(tokens_avec_fichiers_etoile,nb_arg_tokens_avec_fichiers_etoile,path); 
                    // else val = cext(tokens,size,path); 
                    val = 127;
                    //write(STDERR_FILENO,"bash: ./nonexistent: No such file or directory\n",50);
                    write(STDERR_FILENO,"bash: ",7);
                    write(STDERR_FILENO,tokens[0],strlen(tokens[0]));
                    write(STDERR_FILENO,": No such file or directory\n",29);
                    // printf("bash: %d: No such file or directory\n",tokens[0]);
                }
                free(save);
                free(tmpToken0);
                free(savePourSecondTok );
            }
            //Si ce n'est pas un chemin vers une commande externe c'est à dire que c'est juste une commande externe
            else{
                //On verifie si c'est une commande externe
                if(existenceCommandeExterne(tokens[0]) == 1){
                    //printf("Avant Chemin normal\n");
                    if (il_ya_eu_etoile){
                        val = cext(tokens_avec_fichiers_etoile,nb_arg_tokens_avec_fichiers_etoile,path); 
                    }
                    else{
                        val = cext(tokens,size,path); 
                    }
                    //printf("Apres Chemin normal\n");
                }
                else{
                    //Commande inexistante

                    val = 127;

                    write(STDERR_FILENO,tokens[0],strlen(tokens[0]));
                    write(STDERR_FILENO," : commande introuvable\n",25);
                    // printf("bash: %d: No such file or directory\n",tokens[0]);
                }
            }
            //On free le tableau avec les fichiers etoiles
            for(int i = 0; i < nb_arg_tokens_avec_fichiers_etoile; i++){
                free(tokens_avec_fichiers_etoile[i]);
            }
            free(tokens_avec_fichiers_etoile);
        }
        //On cherche le nombre de caractère de val
        char valeur[5];
        sprintf(valeur,"%d",val);
        int taille_val= strlen(valeur);
        //Pour mettre la variable chemin à jour
        tronquageA30Characteres(path->data,chemin,val,taille_val);
        // readline fait un malloc à chaque fois donc on dois le free à la fin
        //free(argv);
        free(tmp);
        free(input);
    }
    //Pour faire un free de path
    string_delete(path);
    string_delete(pathBefore);
    return val;
}

