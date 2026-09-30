#include <stdio.h>
#include <stdlib.h>
#include "liste.h"
#include <time.h>

#define MAX_BUFFER_SIZE 100
#define MAX_NOMS 1000
#define MAX_PRENOMS 1000

char* retirerAccents(const char* texte);
char* normaliserTexte(const char* texte);
char* genererNom(char* fichierNoms);
void supprimerNouvelleLigne(char* chaine);
void chargerContacts(Liste_niveaux* liste, FILE* fichier);
char* genererPrenom(char* fichierPrenoms);
void genererFichierNomPrenom(char* fichierNoms, char* fichierPrenoms, char* fichierSortie, int nombreCombinaisons);
void ajouterRendezVous(Contact* contact, const char* date, const char* heure, const char* objetRdv);
void sauvegarderContacts(FILE* fichier, Liste_niveaux* liste);