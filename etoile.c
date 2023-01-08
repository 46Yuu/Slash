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
#define MAX_TOKEN_ETOILE 500 //Nombre max de fichier récuperable avec les étoiles


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
     * size -> la taille du tableau
     * chemin -> c'est le rep ou se trouve etoile sois dans le courant sois apres un deplacement
     * argv -> c'est le tableau pour stocker les fichiers trouvé
     * nb_argv -> c'est la taille du tableau pour stocker les fichiers, 0 au depart
     * commenceParSlash -> Pour le cas où le chemin commence par slash
     */
int etoile(char ** args, int * size_trouver, char * chemin,char ** argv,int *nb_argv,int commenceParSlash){


    //variables
    int size = *size_trouver;
    int k =0;
    char * repCourant = malloc(200*sizeof(char));
    getcwd(repCourant,200);
        
    char * buf = malloc(PATH_MAX*sizeof(char));
    //memset(buf,0,sizeof(char)*PATH_MAX);

    DIR * dir = NULL;
    struct dirent * entry = NULL;
    struct  stat st;
    struct  stat lst;
    int result_stat;
    int result_lstat;
    
   ici:
   
    if(strcmp(chemin,"")==0)
    {
      //Si on est quelque part au milieu du chemin
      if(!commenceParSlash) dir =opendir(".");

      //Cas du premier tour de boucle ou chemin est vide et le chemin commence par slash
      else dir=opendir(repCourant);

    }else{//Cas où du tour de boucle n>= 1 où on a un chemin du type a/b/...

      //Si le chemin commence par slash alors on met slash au début
      if(commenceParSlash == 1){
        // strcpy(buf, chemin);
        // chemin[0] = '\0';
        // strcat(chemin,"/");
        // strcat(chemin,buf);
        strcpy(buf, "/");
        strcat(buf, chemin);
        strcpy(chemin,buf);
      }
      result_stat = stat(chemin,&st);
      if( result_stat==-1){

        result_lstat = lstat(chemin,&lst);

        //Le cas où ni stat ni lstat ne marche
        if( result_lstat ==-1){
          // if((strcmp(chemin,"./commands/deck") == 0) || (strcmp(chemin,"./words/deck") == 0) ){
          //   free(buf);
          //   free(repCourant);
          //   return 1;
          // }
          
          free(buf);
          free(repCourant);
          //printf("Ni stat ni lstat ne marche sur %s et slash est %d\n",chemin,commenceParSlash);
          return 1;
            

        }
        readlink(chemin,buf,PATH_MAX);
        printf("buf est %s\n et chemin est %s\n",buf,chemin);
        dir = opendir(buf);

      }else{
        dir=opendir(chemin); 
      }
    }

    if (dir == NULL){ 
      printf("erreur de rep ");
      goto error;
    }

 
    // ------------------CAS 1------------------  
    //cmd sans chemin(rep courant) || arriver au dernier * de la chaine || cmd **
    if (size == 1)  //CAS  cmd *.extention \ cmd * 
    {
        while ((entry=readdir(dir)))
        { 
          

          if(strcmp(chemin,"")==0){
            //
            sprintf(buf,"%s",entry->d_name); // on stock le nom de fichier trouver dans buf
          }
          else{

            //printf("Arrivé au else\n");
            //
            sprintf(buf,"%s/%s",chemin,entry->d_name); // on stock le chemin avec le nom de fichier trouver dans buf 
            ///printf("buf est %s\n",buf);

          }

          if(stat(buf,&st)==-1){
            printf("Le stat a pas marché\n");
            goto error;
          }

          char tmp_dname [1] = "";        // le premier caractere des noms  de fichiers
          tmp_dname[0] = entry->d_name[0];// pour comparer avec les dossier cacher caractere .

             //TROIS CAS POSSIBLES
          if(strcmp(args[k],"**")==0){ // cmd ** 

            if ((strcmp(tmp_dname,".")!=0)&& S_ISDIR(st.st_mode)){
 
                argv[*nb_argv]=malloc(MAX_TAILLE_NOM_FICHIER * sizeof(char)+1); 
                // test malloc
                if(argv[*nb_argv] == NULL){
                    printf("Malloc a pas marché\n");
                    free(argv[*nb_argv]);
                    goto error;
                        
                }
                sprintf(argv[*nb_argv],"%s",buf); // on mets le contenu de buf dans le tableau
                (*nb_argv)++;     
                
               etoile(args,&size,buf,argv,nb_argv); // recursion sur le nouveau rep          

            }
          }

          if(strcmp(args[k],"*")==0){ // cmd * 

            if ((strcmp(tmp_dname,".")!=0) && (strcmp(entry->d_name,".")!=0) && (strcmp(entry->d_name,"..")!=0)){ // pas un dossier cacher
                
                argv[*nb_argv]=malloc(MAX_TAILLE_NOM_FICHIER * sizeof(char)+1); 
                // test malloc
                if(argv[*nb_argv] == NULL){
                    printf("Malloc a pas marché\n");
                    free(argv[*nb_argv]);
                    goto error;
                        
                }
                sprintf(argv[*nb_argv],"%s",buf); // on mets le contenu de buf dans le tableau
                (*nb_argv)++;
            }
          }
          else //  *. avec extention
          {

            char * extention = args[k];
            strtok(extention,"*"); // recuperer l"extention si y en a pour la commparer avec la fin de tous les fichiers

            if((strcmp(tmp_dname,".")!=0) && (strcmp(entry->d_name,".")!=0) && (strcmp(entry->d_name,"..")!=0)&&(suffix(extention+1,entry->d_name)==1)){  
                //printf("Enfin dans le if\n");

                argv[*nb_argv]=malloc(MAX_TAILLE_NOM_FICHIER * sizeof(char)+1); 
                // test malloc
                if(argv[*nb_argv] == NULL){
                    printf("Malloc a pas marché\n");
                    free(argv[*nb_argv]);
                    goto error;
                                           
                }
                
                sprintf(argv[*nb_argv],"%s",buf); // on mets le contenu de buf dans le tableau
                (*nb_argv)++; 
            }
         } 
              
        }
    }
    // CAS 2
    else{ // size >1 cas ou y a un chemin exemple a/*/b 

         //deux cas sois on est dans un rep precis sois dans * (tous les rep)
        char * res = strstr(args[k],"*");

        if (res!=NULL){ // cas ou le repertoire contient etoile (on tombe sur *  dans le parcours du chemin) || **
            
          char * extention = args[k];
          strtok(extention,"*"); // recuperer l"extention si y'en a pour la commparer avec la fin de tous les fichiers
           
          // TROIS CAS 

          if (strcmp(args[k],"**")==0){

             while ((entry=readdir(dir)))
             {

                if(strcmp(chemin,"")==0){
              //
                   sprintf(buf,"%s",entry->d_name); // on stock le nom de fichier trouver dans buf
                } 
                else {

                  sprintf(buf,"%s/%s",chemin,entry->d_name); // on stock le chemin avec le nom de fichier trouver dans buf 
                }
                if(stat(buf,&st)==-1){
                    printf("Le stat a pas marché iciiii\n");
                    goto error;
                 }
                 // le premier caractere des noms  de fichiers
                char tmp_dname [1] = "";
                tmp_dname[0] = entry->d_name[0];

                 if(S_ISDIR(st.st_mode)&&(strcmp(tmp_dname,".")!=0)){  
                    etoile(args,&size,buf,argv,nb_argv);}
                    if(!S_ISDIR(st.st_mode)&&strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&(strcmp(tmp_dname,".")!=0)){
                      size --;
                      etoile(&args[k+1],&size,chemin,argv,nb_argv);
                    }

                   
                }
             }
             
            

          
          if(strcmp(args[k],"*")==0){ //  /*/
             
             //k++;
             args=&args[k+1];
             size --;

             while ((entry=readdir(dir)))
             {

                if(strcmp(chemin,"")==0){
              //
                   sprintf(buf,"%s",entry->d_name); // on stock le nom de fichier trouver dans buf
                } 
                else {

                  //printf("Arrivé au else\n");
                  //
                  sprintf(buf,"%s/%s",chemin,entry->d_name); // on stock le chemin avec le nom de fichier trouver dans buf 
                  ///printf("buf est %s\n",buf);

                }
                if(stat(buf,&st)==-1){
                    printf("Le stat a pas marché iciiii\n");
                    goto error;
                }
                 // le premier caractere des noms  de fichiers
                char tmp_dname [1] = "";
                tmp_dname[0] = entry->d_name[0];

                if(S_ISDIR(st.st_mode)&&(strcmp(tmp_dname,".")!=0)&&(strcmp(entry->d_name,".")!=0) && (strcmp(entry->d_name,"..")!=0)){
                  etoile(args,&size,buf,argv,nb_argv,0); // recursion sur le nouveau rep
                   
                }
             
             }
                
            }  
            else{ // /*quelquechose/
             
             //k++;
             args=&args[k+1];
             size --;

             while ((entry=readdir(dir))){
                if(strcmp(chemin,"")==0){
                  //
                   sprintf(buf,"%s",entry->d_name); // on stock le nom de fichier trouver dans buf
                } 
                else {

                  //printf("Arrivé au else\n");
                  //
                  sprintf(buf,"%s/%s",chemin,entry->d_name); // on stock le chemin avec le nom de fichier trouver dans buf 
                  ///printf("buf est %s\n",buf);

                }
                if(stat(buf,&st)==-1){
                    printf("Le stat a pas marché iciiii\n");
                    goto error;
                 }
                 // le premier caractere des noms  de fichiers
                char tmp_dname [1] = "";
                tmp_dname[0] = entry->d_name[0];
                
                
                if(S_ISDIR(st.st_mode)&&(strcmp(tmp_dname,".")!=0)&&(strcmp(entry->d_name,".")!=0) && (strcmp(entry->d_name,"..")!=0)){

                  if((suffix(extention+1,entry->d_name)==1)){
                    etoile(args,&size,buf,argv,nb_argv,0); // recursion sur le nouveau rep
                  }
                } 
             
             }
            }
        }

         else { // chemin sans etoile

                
           if((strcmp(chemin,"")!=0))  strcat(chemin,"/"); 
            
            strcat(chemin,args[k]);
            size = size-1;
            k++; // avancer dans les args 
            //args=&args[k+1];
            
            closedir(dir);// fermuture de l'actuel
 
            goto ici;     
          // }  
        }        
    }
    closedir(dir);
    free(buf);

    //printf("Fin etoile extension\n");
    return 0;  

    error : {
    perror("erreur"); 
    closedir(dir);
    free(buf);
    return 1; 
    }
      
   
 }
    


   