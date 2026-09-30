#include "Fichier.h"
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include <time.h>

#define MAX_BUFFER_SIZE 100
#include <stdio.h>
#include <stdlib.h>




char* retirerAccents(const char* texte) {
    // Allouer une mémoire pour la nouvelle chaîne sans accents
    size_t taille = strlen(texte) + 1;
    char* sansAccents = (char*)malloc(taille);

    for (size_t i = 0; i < taille; i++) {
        sansAccents[i] = tolower(sansAccents[i]);
    }
    if (sansAccents == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // Copier caractère par caractère en retirant les accents
    for (size_t i = 0; i < taille; i++) {
        unsigned char c = (unsigned char)texte[i];
        switch (c) {
        case 0xC3: // UTF-8 pour les caractères avec accent
            i++;
            switch (texte[i]) {
            case '\xA0': sansAccents[i - 1] = 'a'; break;  // à
            case '\xA1': sansAccents[i - 1] = 'a'; break;  // á
            case '\xA2': sansAccents[i - 1] = 'a'; break;  // â
            case '\xA8': sansAccents[i - 1] = 'e'; break;  // è
            case '\xA9': sansAccents[i - 1] = 'e'; break;  // é
            case '\xAA': sansAccents[i - 1] = 'e'; break;  // ê
            case '\xAF': sansAccents[i - 1] = 'i'; break;  // ï
            case '\xB4': sansAccents[i - 1] = 'o'; break;  // ô
            case '\xB9': sansAccents[i - 1] = 'u'; break;  // ù
            case '\xBA': sansAccents[i - 1] = 'u'; break;  // ú
            case '\xBB': sansAccents[i - 1] = 'u'; break;  // û

                // Ajoutez d'autres cas pour les autres caractères accentués
            default: sansAccents[i - 1] = c; break; // conserve le caractère d'origine s'il n'est pas accentué
            }
            break;
            // Ajoutez d'autres cas pour d'autres caractères spéciaux si nécessaire
        default: sansAccents[i] = c; break;
        }
    }

    return sansAccents;
}
// Fonction pour générer un nom aléatoire à partir du fichier de noms
char* normaliserTexte(const char* texte) {
    // Retirer les accents puis convertir en minuscules
    char* sansAccents = retirerAccents(texte);
    size_t taille = strlen(sansAccents) + 1;
    char* normalise = (char*)malloc(taille);

    for (size_t i = 0; i < taille; i++) {
        normalise[i] = tolower(sansAccents[i]);
    }

    free(sansAccents);

    return normalise;
}

// Fonction pour générer un nom aléatoire à partir du fichier de noms
char* genererNom(char* fichierNoms) {
    FILE* file = fopen(fichierNoms, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de noms");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE];
    int nombreNoms = 0;

    // Compter le nombre de noms dans le fichier
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        nombreNoms++;
    }

    // Réinitialiser le curseur de fichier
    rewind(file);

    // Générer un index aléatoire pour choisir un nom
    int index = rand() % nombreNoms;

    // Récupérer le nom correspondant à l'index choisi et le normaliser
    char* nom = NULL;
    for (int i = 0; i <= index; i++) {
        fgets(buffer, sizeof(buffer), file);
        if (i == index) {
            // Supprimer le caractère de saut de ligne à la fin du nom
            buffer[strcspn(buffer, "\n")] = '\0';
            nom = buffer;
        }
    }

    fclose(file);
    return nom;
}

// Fonction pour générer un prénom aléatoire à partir du fichier de prénoms
char* genererPrenom(char* fichierPrenoms) {
    FILE* file = fopen(fichierPrenoms, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de prénoms");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE];
    int nombrePrenoms = 0;

    // Compter le nombre de prénoms dans le fichier
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        nombrePrenoms++;
    }

    // Réinitialiser le curseur de fichier
    rewind(file);

    // Générer un index aléatoire pour choisir un prénom
    int index = rand() % nombrePrenoms;

    // Récupérer le prénom correspondant à l'index choisi et le normaliser
    char* prenom = NULL;
    for (int i = 0; i <= index; i++) {
        fgets(buffer, sizeof(buffer), file);
        if (i == index) {
            // Supprimer le caractère de saut de ligne à la fin du prénom
            buffer[strcspn(buffer, "\n")] = '\0';
            prenom = buffer;
        }
    }

    fclose(file);
    return prenom;
}

// Fonction principale pour générer un fichier txt au format nom_prenom
void genererFichierNomPrenom(char* fichierNoms, char* fichierPrenoms, char* fichierSortie, int nombreCombinaisons) {
    FILE* file = fopen(fichierSortie, "w");
    if (file == NULL) {
        perror("Erreur lors de la création du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    // Initialiser le générateur de nombres aléatoires avec une graine basée sur l'heure actuelle
    srand((unsigned int)time(NULL));

    for (int i = 0; i < nombreCombinaisons; i++) {
        char* nom = normaliserTexte(genererNom(fichierNoms));
        char* prenom = normaliserTexte(genererPrenom(fichierPrenoms));

        // Écrire la combinaison dans le fichier de sortie
        fprintf(file, "%s_%s\n", nom, prenom);

        // Libérer la mémoire allouée dynamiquement
        free(nom);
        free(prenom);
    }

    fclose(file);
}
// Implémentation de la fonction de sauvegarde
void sauvegarderContacts(FILE* fichier, Liste_niveaux* liste) {
    if (fichier == NULL || liste == NULL) {
        fprintf(stderr, "Erreur : Fichier ou liste NULL.\n");
        return;
    }

    Contact* contactCourant = liste->tete[0];

    while (contactCourant != NULL) {
        // Sauvegarder le nom du contact
        fprintf(fichier, "%s\n", contactCourant->nom_prenom);

        // Sauvegarder les rendez-vous du contact
        RendezVous* rdvCourant = contactCourant->listeRendezVous;

        while (rdvCourant != NULL) {
            fprintf(fichier, "Rendez-vous : Date : %s, Heure : %s, Objet : %s\n",
                rdvCourant->date, rdvCourant->heure, rdvCourant->ObjetRdv);
            rdvCourant = rdvCourant->suivant;
        }

        // Passer au contact suivant
        contactCourant = contactCourant->niveaux_suivants;
    }
    fclose(fichier);
}

// Fonction pour charger tous les contacts et leurs rendez-vous depuis un fichier
void supprimerNouvelleLigne(char* chaine) {
    // Trouver la position du caractère de nouvelle ligne
    char* positionNouvelleLigne = strpbrk(chaine, "\r\n");

    // Si le caractère de nouvelle ligne est trouvé, le remplacer par '\0'
    if (positionNouvelleLigne != NULL) {
        *positionNouvelleLigne = '\0';
    }
}
void chargerContacts(Liste_niveaux* liste, FILE* fichier) {
    if (fichier == NULL || liste == NULL) {
        fprintf(stderr, "Erreur : Fichier ou liste NULL.\n");
        return;
    }

    char ligne[256];  // Ajustez la taille selon vos besoins

    Contact* contactActuel = NULL;
    int ligneEnAttente = 0; // 1 si la boucle des rendez-vous a déjà lu le nom du contact suivant

    while (ligneEnAttente || fgets(ligne, sizeof(ligne), fichier) != NULL) {
        ligneEnAttente = 0;
        // Supprimer le saut de ligne
        char* debutNom = strtok(ligne, "\r\n");

        // Vérifier si la ligne contient un nom de contact
        if (debutNom != NULL) {
            // Créer un nouveau contact
            contactActuel = CreeCellule(debutNom);
            // Ajouter le contact à la liste
            insererEnOrdre(contactActuel, liste, 0);
        }

        // Lire les rendez-vous du contact
        while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
            if (strncmp(ligne, "Rendez-vous", strlen("Rendez-vous")) != 0) {
                ligneEnAttente = 1; // c'est le contact suivant : on le garde pour le prochain tour
                break;
            }
            char* date = strstr(ligne, "Date : ") + strlen("Date : ");
            char* heure = strstr(ligne, "Heure : ") + strlen("Heure : ");
            char* objetRdv = strstr(ligne, "Objet : ") + strlen("Objet : ");


            // Retirer le saut de ligne à la fin des chaînes
            supprimerNouvelleLigne(date);
            supprimerNouvelleLigne(heure);
            supprimerNouvelleLigne(objetRdv);

            // Créer un nouveau rendez-vous
            RendezVous* nouveauRdv = CreeRendezVous();
            // Limiter la longueur de la chaîne date à 10 caractères
            strncpy(nouveauRdv->date, date, 10);
            // Ajouter le caractère nul de fin de chaîne
            nouveauRdv->date[10] = '\0';

            // Limiter la longueur de la chaîne heure à 5 caractères
            strncpy(nouveauRdv->heure, heure, 5);
            // Ajouter le caractère nul de fin de chaîne
            nouveauRdv->heure[5] = '\0';
            strcpy(nouveauRdv->ObjetRdv, objetRdv);

            // Ajouter le rendez-vous au contact
            insererEnTeteRendezVous(contactActuel, nouveauRdv);
        }
    }
}

// Ajouter un rendez-vous à un contact
void ajouterRendezVous(Contact* contact, const char* date, const char* heure, const char* objetRdv) {
    if (contact == NULL || date == NULL || heure == NULL || objetRdv == NULL) {
        fprintf(stderr, "Erreur : Paramètre NULL.\n");
        return;
    }

    // Créer un nouveau rendez-vous
    RendezVous* nouveauRdv = (RendezVous*)malloc(sizeof(RendezVous));
    if (nouveauRdv == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer de la mémoire pour le rendez-vous.\n");
        return;
    }

    // Copier les informations du rendez-vous
    nouveauRdv->date = _strdup(date);
    nouveauRdv->heure = _strdup(heure);
    nouveauRdv->ObjetRdv = _strdup(objetRdv);

    // Ajouter le rendez-vous à la liste du contact
    nouveauRdv->suivant = contact->listeRendezVous;
    contact->listeRendezVous = nouveauRdv;
}