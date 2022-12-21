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
#define PATH_COMMANDE_EXTERNE "/usr/bin"


//Tronquage du chemin à afficher à 30 charactères
void tronquageA30Characteres(char * data, char * cheminA30Caracteres, int val){
    int len = strlen(data);
    //printf("trunc\n");
    cheminA30Caracteres[0] = '\0';
    char valeurRetour[2];
    sprintf(valeurRetour,"%d",val);
    strcat(cheminA30Caracteres,"[");
    if(val == 0){
        strcat(cheminA30Caracteres,"\001\033[32m\002");
    }
    else {
        strcat(cheminA30Caracteres,"\001\033[91m\002");
    }
    strcat(cheminA30Caracteres,valeurRetour);
    strcat(cheminA30Caracteres,"\001\033[00m\002");
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


//Verifie si la commande externe existe c'est à dire si elle est dans le fichier /usr/bin
int existenceCommandeExterne(char * cmd){
    int retour = 0;
    //printf("La commande est %s",cmd);
    DIR * dir = opendir(PATH_COMMANDE_EXTERNE);
    if(dir == NULL){
        return -1;
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
            tokens[i] = token;
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
    //printf("Overture réussi\n");

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

int main(int argc, char **argv) {
    printf("------------------------Test de Make sur slash-------------\n");

    signal(SIGINT,SIG_IGN);
    signal(SIGTERM,SIG_IGN);
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
        }else{
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
                    tokens[0] = tokens0[tailleApresSeparation - 1];
                    //printf("Bref tokens[0] est %s \n",tokens[0]);


                    val = cext(tokens,size,path); 
                }else{//Le cas où le chemin est incorrecte on renvoie la valeur d'erreur 1

                    //val = 1;
                }
                free(save);
                free(tmpToken0);
                free(savePourSecondTok );

            }//Le cas où c'est juste une commande externe
            /*else if(existenceCommandeExterne(tokens[0]) == 1){
                 if (size >1){
                    for (int i=0;i<= size;i++){
                        if(strstr(tokens[i],"*")){  
                            char * repEtoile =malloc(PATH_MAX*sizeof(char));
                            memset(repEtoile,0,PATH_MAX*sizeof(char));  
                            int t = 0;
                            int * taillePatherne = &t;
                            char ** patherne = tokage(tokens[i],'/',taillePatherne);
                            //printf("taille est %d\n",*taillePatherne);
                            //for(int i=0;i< *taillePatherne;i++){
                            //    printf("%s\n",patherne[i]);
                            //}
                            etoile(patherne,taillePatherne,repEtoile); 
                            free(patherne);
                            free(repEtoile);
                        }  
                    }
                }else { 

                    // val = cext(tokens,size,path);
                }   

            }/*else{
                //val est 1 dans tous les autres cas
                //val = 1;
            }*/
            else{
                val = cext(tokens,size,path);
            }
 

        }
            
        

        //Pour mettre la variable chemin à jour
        tronquageA30Characteres(path->data,chemin,val);

        // readline fait un malloc à chaque fois donc on dois le free à la fin
        free(tmp);
        free(input);
 
    }

    //Pour faire un free de path
    string_delete(path);
    return val;
}

