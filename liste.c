#include "liste.h"
#include <string.h>
#include <ctype.h>
// Fonction pour créer une nouvelle liste dynamique
Liste_niveaux* creeliste(int niveauMax) {
    Liste_niveaux* liste = (Liste_niveaux*)malloc(sizeof(Liste_niveaux));
    liste->niveauMax = niveauMax;
    liste->tete = (Contact**)malloc(niveauMax * sizeof(Contact*));

    // Initialiser la tête de chaque niveau à NULL
    for (int i = 0; i < niveauMax; ++i) {
        liste->tete[i] = NULL;
    }

    return liste;
}
void insererEnTete(Contact* cell, Liste_niveaux* liste, int niveau)
{

    // Vérifier si le niveau est valide
    if (niveau < 0 || niveau >= liste->niveauMax) {
        fprintf(stderr, "Erreur : Niveau invalide.\n");
        return;
    }

    // Vérifier si la mémoire a été allouée pour la liste et le niveau
    if (liste->tete == NULL || niveau >= liste->niveauMax) {
        fprintf(stderr, "Erreur : Allocation mémoire incorrecte pour la liste ou le niveau.\n");
        return;
    }

    // Vérifier si la mémoire a été allouée pour la Contact et les niveaux suivants
    if (cell->niveaux_suivants == NULL) {
        cell->niveaux_suivants = (Contact*)malloc(liste->niveauMax * sizeof(Contact*));

        if (cell->niveaux_suivants == NULL) {
            fprintf(stderr, "Erreur : Impossible d'allouer de la mémoire pour les niveaux suivants de la Contact.\n");
            return;
        }
    }

    // Insérer la Contact en tête du niveau spécifié
    cell->niveaux_suivants = liste->tete[niveau];
    liste->tete[niveau] = cell;
}


void insererEnOrdre(Contact* cell, Liste_niveaux* liste, int niveau) {
    if (cell == NULL || liste == NULL) {
        fprintf(stderr, "Erreur : Pointeur NULL détecté.\n");
        return;
    }

    if (niveau < 0 || niveau >= liste->niveauMax) {
        fprintf(stderr, "Erreur : Niveau invalide.\n");
        return;
    }

    Contact* prec = NULL;
    Contact* curr = liste->tete[niveau];

    // Trouver l'emplacement d'insertion (ordre lexicographique)
    while (curr != NULL) {
        const char* nomCell = cell->nom_prenom;
        const char* nomCurr = curr->nom_prenom;

        // Parcourir les caractères pour comparer
        while (*nomCell != '\0' && *nomCurr != '\0' && *nomCell == *nomCurr) {
            nomCell++;
            nomCurr++;
        }

        // Si les caractères différent ou si le nomCell est inférieur au nomCurr, insérer la cellule
        if (*nomCell == '\0' || (*nomCell < *nomCurr)) {
            break;
        }

        prec = curr;
        curr = curr->niveaux_suivants;
    }

    // Insérer la Contact en ordre
    if (prec == NULL) {
        // Insertion en tête
        cell->niveaux_suivants = liste->tete[niveau];
        liste->tete[niveau] = cell;
    }
    else {
        // Insertion ailleurs dans la liste
        prec->niveaux_suivants = cell;
        cell->niveaux_suivants = curr;
    }
}


Liste_niveaux* construireListe_niveaux(int n,Liste_niveaux* ListeContact) {

    // on remet toute les tetes superieur à 0 pour pas avoir de bug memoire
    ListeContact->tete[1] = NULL;
    ListeContact->tete[2] = NULL;
    ListeContact->tete[3] = NULL;
    for (int niveau = 1; niveau < n; ++niveau) {
        Contact* courant = ListeContact->tete[0];
        Contact* nouvelleTete = NULL;
        Contact* dernierAjoute = NULL;
        if (courant != NULL)
        {

            courant = courant->niveaux_suivants;
        }

        while (courant != NULL) {
            Contact* nouvelleCellule = CreeCellule(courant->nom_prenom);

            if (courant->niveaux_suivants != NULL)
            {
                Contact* CelluleSuivante = CreeCellule(courant->niveaux_suivants->nom_prenom);
                while (ComparerContactSelonNiveau(nouvelleCellule->nom_prenom, CelluleSuivante->nom_prenom, niveau) == 0)
                {
                    if (courant->niveaux_suivants->niveaux_suivants != NULL)
                    {
                        courant = courant->niveaux_suivants;
                        nouvelleCellule = CreeCellule(courant->nom_prenom);
                        CelluleSuivante = CreeCellule(courant->niveaux_suivants->nom_prenom);
                    }
                    else
                    {
                        break;
                    }

                }
            }
            RendezVous* rdvCourant = courant->listeRendezVous;
            while (rdvCourant != NULL) {
                RendezVous* nouveauRdv = CreeRendezVous();
                strcpy(nouveauRdv->date, rdvCourant->date);
                strcpy(nouveauRdv->heure, rdvCourant->heure);
                strcpy(nouveauRdv->ObjetRdv, rdvCourant->ObjetRdv);
                insererEnTeteRendezVous(nouvelleCellule, nouveauRdv);
                rdvCourant = rdvCourant->suivant;
            }



            insererEnOrdre(nouvelleCellule, ListeContact, niveau);
            if (dernierAjoute == NULL) {
                nouvelleTete = nouvelleCellule;
            }
            else {
                dernierAjoute->niveaux_suivants = nouvelleCellule;
            }
            dernierAjoute = nouvelleCellule;
            courant = courant->niveaux_suivants;


        }

        // Mettre à jour la tête du niveau actuel
        ListeContact->tete[niveau] = nouvelleTete;

    }

    return ListeContact;
}


Contact* rechercheClassique(Liste_niveaux* liste, char* Nom_Prenom) {
    if (liste == NULL || liste->tete == NULL) {
        return NULL;
    }

    Contact* ContactActuel = liste->tete[0];

    while (ContactActuel != NULL) {
        if (strcmp(ContactActuel->nom_prenom, Nom_Prenom) == 0) {
            return ContactActuel;
        }
        ContactActuel = ContactActuel->niveaux_suivants;
    }

    return NULL;
}

// Fonction de recherche à partir du niveau le plus haut
Contact* rechercheContact(Liste_niveaux* liste, char* Nom_Prenom) {
    if (liste == NULL || Nom_Prenom == NULL) {
        fprintf(stderr, "Erreur : Liste ou nomPrenom NULL.\n");
        return NULL;
    }

    int niveau = liste->niveauMax - 1;
    Contact* contactTrouve = NULL;

    // Parcourir les niveaux en descendant jusqu'au niveau 0
    while (niveau >= 0 && contactTrouve == NULL) {
        Contact* contactActuel = liste->tete[niveau];

        // Parcourir la liste du niveau actuel
        while (contactActuel != NULL && contactTrouve == NULL) {
            // Comparer le nomPrenom avec le nomPrenom du contact actuel
            if (strcmp(contactActuel->nom_prenom, Nom_Prenom) == 0) {
                contactTrouve = contactActuel;
            }

            contactActuel = contactActuel->niveaux_suivants;
        }

        niveau--;
    }

    return contactTrouve;
}

void afficherContact(Contact* contact) {
    if (contact == NULL) {
        return;
    }

    printf("Nom : %s\n", contact->nom_prenom);
    printf("------------------------------\n");

    // Ajoutez d'autres informations sur le contact au besoin
}

int rechercheAutoAvecAffichage(Liste_niveaux* liste, char* saisie) {
    if (liste == NULL || saisie == NULL || strlen(saisie) < 3) {
        fprintf(stderr, "Erreur : Liste ou saisie NULL ou charactere inferieur a 3.\n");
        return 0;
    }

    printf("\nRésultats de la recherche pour : %s\n", saisie);

    // Parcourir les niveaux en partant du plus haut
    for (int niveau = liste->niveauMax - 1; niveau >= 0; --niveau) {
        Contact* courant = liste->tete[niveau];

        while (courant != NULL) {
            int i = 0;
            int match = 1;

            // Comparer caractère par caractère jusqu'à la fin de la saisie
            while (saisie[i] != '\0') {
                if (courant->nom_prenom[i] != saisie[i]) {
                    match = 0;
                    break;
                }
                i++;
            }

            // Si la saisie correspond aux premiers caractères du nom, afficher le contact
            if (match) {
                afficherContact(courant);
                return 0;
            }

            courant = courant->niveaux_suivants;
        }
    }
    printf("Aucun contact trouvé.\n");
    return 0;
}



void supprimerRendezVous(Contact* contact, int numeroRdvASupprimer) {
    if (contact == NULL) {
        fprintf(stderr, "Erreur : Contact NULL.\n");
        return;
    }

    RendezVous* rdvCourant = contact->listeRendezVous;
    RendezVous* rdvPrecedent = NULL;
    int numeroRdv = 1;

    // Parcourir les rendez-vous du contact
    while (rdvCourant != NULL) {
        if (numeroRdv == numeroRdvASupprimer) {
            // Supprimer le rendez-vous
            if (rdvPrecedent == NULL) {
                // Le rendez-vous à supprimer est en tête de liste
                contact->listeRendezVous = rdvCourant->suivant;
            }
            else {
                // Le rendez-vous à supprimer n'est pas en tête de liste
                rdvPrecedent->suivant = rdvCourant->suivant;
            }

            // Libérer la mémoire du rendez-vous supprimé
            // Libérer la mémoire du rendez-vous supprimé
            free(rdvCourant->date);
            free(rdvCourant->heure);
            free(rdvCourant->ObjetRdv);
            free(rdvCourant);


            printf("Le rendez-vous a été supprimé avec succès.\n");
            return;
        }

        rdvPrecedent = rdvCourant;
        rdvCourant = rdvCourant->suivant;
        numeroRdv++;
    }

    fprintf(stderr, "Erreur : Le rendez-vous demandé n'existe pas.\n");
}

void afficherRendezVous(Contact* contact) {
    if (contact == NULL) {
        fprintf(stderr, "Erreur : Contact NULL.\n");
        return;
    }

    printf("Rendez-vous pour %s :\n", contact->nom_prenom);

    // Vérifier si le contact a des rendez-vous
    if (contact->listeRendezVous == NULL) {
        printf("Aucun rendez-vous.\n");
        return;
    }

    RendezVous* rdvCourant = contact->listeRendezVous;

    // Afficher chaque rendez-vous, numéroté (le numéro sert à la suppression)
    int numero = 1;
    while (rdvCourant != NULL) {
        printf("[%d] Date : %s, Heure : %s, Objet : %s\n", numero++, rdvCourant->date, rdvCourant->heure, rdvCourant->ObjetRdv);
        rdvCourant = rdvCourant->suivant;
    }
}

// Fonction de recherche de sous-chaîne insensible à la casse
const char* strcasestr_custom(const char* haystack, const char* needle) {
    if (!haystack || !needle) {
        return NULL;
    }

    // Convertir la chaîne d'entrée et la sous-chaîne en minuscules
    char haystackLower[256];  // Ajustez la taille selon vos besoins
    char needleLower[256];    // Ajustez la taille selon vos besoins

    size_t i = 0;
    while (haystack[i] != '\0' && i < sizeof(haystackLower) - 1) {
        haystackLower[i] = tolower((unsigned char)haystack[i]);
        i++;
    }
    haystackLower[i] = '\0';

    i = 0;
    while (needle[i] != '\0' && i < sizeof(needleLower) - 1) {
        needleLower[i] = tolower((unsigned char)needle[i]);
        i++;
    }
    needleLower[i] = '\0';

    // Effectuer la recherche avec strstr
    return strstr(haystackLower, needleLower);
}

RendezVous* CreeRendezVous() {
    // Allouer de la mémoire pour le nouveau rendez-vous
    RendezVous* nouveauRdv = (RendezVous*)malloc(sizeof(RendezVous));

    if (nouveauRdv == NULL) {
        perror("Erreur d'allocation mémoire pour le rendez-vous");
        exit(EXIT_FAILURE);
    }

    // Initialiser les champs du rendez-vous
    nouveauRdv->date = NULL;
    nouveauRdv->heure = NULL;
    nouveauRdv->ObjetRdv = NULL;
    nouveauRdv->suivant = NULL;

    // Allouer de la mémoire pour les champs du rendez-vous
    nouveauRdv->date = (char*)malloc(TAILLE_DATE * sizeof(char));
    nouveauRdv->heure = (char*)malloc(TAILLE_HEURE * sizeof(char));
    nouveauRdv->ObjetRdv = (char*)malloc(TAILLE_OBJET * sizeof(char));

    if (nouveauRdv->date == NULL || nouveauRdv->heure == NULL || nouveauRdv->ObjetRdv == NULL) {
        perror("Erreur d'allocation mémoire pour les champs du rendez-vous");
        exit(EXIT_FAILURE);
    }

    return nouveauRdv;
}

void insererEnTeteRendezVous(Contact* contact, RendezVous* nouveauRdv) {
    if (contact == NULL || nouveauRdv == NULL) {
        fprintf(stderr, "Erreur : Pointeur NULL détecté.\n");
        return;
    }

    // Insérer le rendez-vous en tête de la liste des rendez-vous du contact
    nouveauRdv->suivant = contact->listeRendezVous;
    contact->listeRendezVous = nouveauRdv;
}
