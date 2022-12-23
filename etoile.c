#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "mystring.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include "etoile.h"

#define PATH_MAX 4096
#define MAX_TAILLE_NOM_FICHIER 50


  /**
   *qui test les derniers caracteres de machaine si ca commence par monsuffix
   *
   */
 int suffix(char * monsuffix, char * machaine){
       if( strcmp(machaine+(strlen(machaine)-strlen(monsuffix)),monsuffix)==0) {return 1;}
       else {
        return 0;

    }
    }
    /**
     * args -> le token ou y'a l'etoile sous forme d'un tableau sans /
     * size -> la taille de se tableau
     * chemin -> c'est le rep ou se trouve etoile sois dans le courant sois apres un deplacement
     * argv -> c'est le tableau pour stocker les fichiers trouvé
     * nb_argv -> c'est la taille du tableau pour stocker les fichiers, 0 au depart
     */
int etoile(char ** args, int * size, char * chemin,char ** argv,int *nb_argv){


    //variables


    int k =0;
    char * repCourant = malloc(PATH_MAX*sizeof(char));
    getcwd(repCourant,PATH_MAX);

    char * buf = malloc(PATH_MAX*sizeof(char));
    memset(buf,0,sizeof(char)*PATH_MAX);

    DIR * dir = NULL;
    struct dirent * entry = NULL;
    struct  stat st;

    // int *nb_argv = 0;   
    // char ** argv = malloc(PATH_MAX*(sizeof(char *)));// argv -> tableau pour stocker les fichiers trouver

   ici: if(strcmp(chemin,"")==0){
      dir=opendir(repCourant);
    }else{
        dir = opendir(chemin);
    }
    if (dir == NULL){ 
        goto error;
    }
    // CAS 1  cmd sans chemin
    if (*size == 1)  //CAS  cmd *.extention \ cmd * sur le rep courant 
    {
        while ((entry=readdir(dir)))
        {
          if(strcmp(chemin,"")!=0){
            sprintf(buf,"%s/%s",chemin,entry->d_name); // on stock le chemin et le noms de fichier trouver dans buf
          }
          else{
            //printf("Arrivé au else\n");
            sprintf(buf,"%s",entry->d_name); 
            ///printf("buf est %s\n",buf);

          }
          if(stat(buf,&st)==-1){
            printf("Le stat a pas marché\n");
            goto error;
          }
          char tmp_dname [1] = "";        // le premier caractere des noms  de fichiers
          tmp_dname[0] = entry->d_name[0];// pour comparer avec les dossier cacher caractere .


          if(strcmp(args[k],"*")==0){ // cmd * 
            if ((strcmp(tmp_dname,".")!=0)){
                
                argv[*nb_argv]=malloc(MAX_TAILLE_NOM_FICHIER * sizeof(char)+1); 
                if(argv[*nb_argv] == NULL){
                    printf("Malloc a pas marché\n");
                    free(argv[*nb_argv]);
                    goto error;
                        
                }
                sprintf(argv[*nb_argv],"%s",buf); // on mets le contenu de buf dans le tableaux
                (*nb_argv)++;
            }
          }else{// cdm *.extention

            char * extention = args[k];
            strtok(extention,"*"); // recuperer l"extention si y en a pour la commparer avec la fin de tous les fichiers

            if((strcmp(tmp_dname,".")!=0)&&(suffix(extention+1,entry->d_name)==1)){  
                //printf("Enfin dans le if\n");
                argv[*nb_argv]=malloc(MAX_TAILLE_NOM_FICHIER * sizeof(char)+1); 
                if(argv[*nb_argv] == NULL){
                    printf("Malloc a pas marché\n");
                    free(argv[*nb_argv]);
                    goto error;
                                           
                }
                sprintf(argv[*nb_argv],"%s",buf); // on mets le contenu de buf dans le tableaux
                (*nb_argv)++; 
                }
            }   
        }
    }
    else{ // size >1 cas ou y a un chemin exemple a/*/b 
         if (strcmp(args[k],"*")==0){ // cas ou le repertoire contient etoile (on tombe sur * dans le parcours du chemin)
            char * extention = args[k];
            strtok(extention,"*"); // recuperer l"extention si y'en a pour la commparer avec la fin de tous les fichiers
            size --;
            args=&args[k+1];
            while ((entry=readdir(dir)))
            {
                if(strcmp(chemin,"")!=0){
                    sprintf(buf,"%s/%s",chemin,entry->d_name); // on stock dans buf
                    printf(buf,"%s",entry->d_name); }
                if(stat(buf,&st)==-1){
                     goto error;
                 }
                char tmp_dname [1] = "";
                tmp_dname[0] = entry->d_name[0];// le premier caractere des noms  de fichiers

                if(S_ISDIR(st.st_mode)&&(strcmp(tmp_dname,".")!=0)){
                    if((suffix(extention,entry->d_name)==1)){
                    etoile(args,size,buf,argv,nb_argv); // recursion sur le nouveau rep
                }
                }
            }
        }
        else{//le rep n'a pas de etoile dedans (on est dans a ou b )
            if((strcmp(chemin,"")!=0) && (strcmp(chemin,"/")!=0)){
                strncat(chemin,"/",2);
            }// on mets a jour le chemin de rep a ouvrir
            strncat(chemin,args[k],strlen(args[k]));
            closedir(dir);// fermuture de l'actuel
            size --;
            k++; // avancer dans les args au nouveau rep 
            goto ici;     
        }          
    }
    closedir(dir);
    free(buf);
    
    //printf("Fin etoile extension\n");
    return 1;  

    error : {
    perror("stat"); 
    free(buf);
    return 0; 
    }
      
   
 }
    


   