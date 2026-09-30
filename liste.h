#include "cellule.h"

#pragma once
#define TAILLE_DATE 12
#define TAILLE_HEURE 7
#define TAILLE_OBJET 30
typedef struct Liste_niveaux {
    int niveauMax;
    Contact** tete;
} Liste_niveaux;

Liste_niveaux* creeliste(int niveauMax);
Liste_niveaux* construireListe_niveaux(int n,Liste_niveaux* ListeContact);
int rechercheAutoAvecAffichage(Liste_niveaux* liste, char* saisie);
void afficherRendezVous(Contact* contact);
Contact* rechercheContact(Liste_niveaux* liste, char* Nom_Prenom);
Contact* rechercheClassique(Liste_niveaux* liste, char* Nom_Prenom);
void insererEnTete(Contact* Contact, Liste_niveaux* liste, int niveau);
void supprimerRendezVous(Contact* contact, int numeroRdvASupprimer);
void insererEnOrdre(Contact* cell, Liste_niveaux* liste, int niveau);
void afficherContact(Contact* contact);
const char* strcasestr_custom(const char* haystack, const char* needle);

void insererEnTeteRendezVous(Contact* contact, RendezVous* nouveauRdv);
RendezVous* CreeRendezVous();
