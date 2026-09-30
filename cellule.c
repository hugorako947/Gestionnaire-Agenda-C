#include "cellule.h"
#include <string.h>
Contact* CreeCellule(char* nom_prenom) {
    Contact* nouvelleCellule = (Contact*)malloc(sizeof(Contact));

    if (nouvelleCellule == NULL) {
        exit(EXIT_FAILURE);
    }

    nouvelleCellule->nom_prenom = _strdup(nom_prenom);
    nouvelleCellule->listeRendezVous = NULL;
    if (nouvelleCellule->nom_prenom == NULL) {
        // Gestion d'erreur pour l'échec de l'allocation mémoire
        free(nouvelleCellule);
        exit(EXIT_FAILURE);
    }

    nouvelleCellule->niveaux_suivants = NULL; // Assurez-vous d'initialiser à NULL si nécessaire

    return nouvelleCellule;
}

int ComparerContactSelonNiveau(char* Nom_Prenom, char* Nom_Prenom2, int niveau)
{
    switch (niveau) {
    case 1:
        for (int i = 0; i < 3; i++) {
            if (Nom_Prenom[i] != Nom_Prenom2[i]) {
                return 1; // Retourner vrai si les chaînes sont différentes
            }
        }
        break;
    case 2:
        if (Nom_Prenom[0] == Nom_Prenom2[0] && Nom_Prenom[1] != Nom_Prenom2[1]) {
            return 1;
        }
        else {
            return 0;
        }
        break;
    case 3:
        if (Nom_Prenom[0] != Nom_Prenom2[0]) {
            return 1;
        }
        else {
            return 0;
        }
        break;
    default:
        return 0; // Retourner 0 par défaut
    }
    return 0;
}
