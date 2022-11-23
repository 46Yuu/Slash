#ifndef MYSTRING_H
#define MYSTRING_H

// La structure string stocke une chaîne de caractères terminée par un \0
// dans un buffer de taille connue, pour éviter les débordements.

struct string {
  size_t capacity; // Taille du buffer .data
  size_t length;   // Taille de la chaîne, sans compter le \0 terminal
  size_t lengthBefore;
  char * dataBefore;
  char * data;     // Contenu de la chaîne, avec un \0 terminal
};

// Crée une nouvelle chaîne de longueur 0 et de capacité donnée, allouée
// sur le tas (c'est-à-dire avec malloc).
struct string * string_new(size_t capacity);

// Détruit une chaîne, en libérant la mémoire occupée.
void string_delete(struct string * str);

// Met à jour dest en ajoutant src à la *fin*.
// Renvoie 1 en cas de réussite, 0 en cas d'échec.
// Échoue sans rien modifier si le buffer n'est pas assez grand.
int string_append (struct string * dest, char * src);

// Tronque la chaîne en enlevant les nchars derniers caractères
// (ou tous les caractères s'il y en a moins de nchars).
void string_truncate (struct string * str, size_t nchars);


//Tronque la chaine jusqu'a trouver le premier "/" pour gérer les chemins
void string_truncate_to_slash (struct string * str);

#endif // MYSTRING_H
