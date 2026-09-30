#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma once
#include <string.h>
#ifndef _WIN32
#define _strdup strdup // _strdup n'existe que sous Windows
#endif

typedef struct RendezVous {
    char* date;
    char* heure;
    char* ObjetRdv;
    struct RendezVous* suivant;// Ajoutez d'autres informations sur le rendez-vous au besoin
} RendezVous;

typedef struct Contact {
    char* nom_prenom;
    int niveaux;
    struct RendezVous* listeRendezVous;
    struct Contact* niveaux_suivants;
} Contact;

Contact* CreeCellule(char* nom_prenom);

int ComparerContactSelonNiveau(char* Nom_Prenom, char* Nom_Prenom2, int niveau);
