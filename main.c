#include "liste.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"
#include "saisie.h"
#include "Fichier.h"
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define TailleT 10

// Vide la fin de la ligne laissée par scanf (sinon le menu lit une ligne vide)
static void viderLigne(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // affiche correctement les accents dans la console Windows
#endif
    /*
    char fichierNoms[] = "noms.txt";
    char fichierPrenoms[] = "prenom.csv";
    char fichierSortie[] = "contacts.txt";
    
    genererFichierNomPrenom(fichierNoms, fichierPrenoms, fichierSortie,1000);
    */
    srand(time(NULL));
    FILE* log_file = fopen("log.txt", "w");
    char format[] = "%s\t%s\t%s\n";
    char* time_lvl0;
    char* time_all_levels;
    FILE* fichierCharger = fopen("contacts.txt", "r");
    char* input;
    Liste_niveaux* liste = creeliste(4);
    chargerContacts(liste, fichierCharger);
   
    liste = construireListe_niveaux(4, liste);
    




    while (1) {
        printf("\n[1]. Rechercher un contact\t\t [2]. Afficher les rendez-vous d'un contact\n[3]. Creer un contact \t\t\t [4]. Creer un rendez-vous pour un contact\n[5]. Supprimer un rendez-vous \t\t [6]. Sauvegarder le fichier de tous les rendez-vous (la liste de rendez-vous & contact se charge automatiquement depuis le fichier contacts.txt )\n[7]. Fournir les temps de calcul pour une insertion de nouveau contact\n[8]. Quitter\n\n");
        printf("Choisissez une option : ");
        input = scanString();

        switch (atoi(input)) {
        case 1:
            printf("Entrez le nom partiel pour la recherche (a partir de la 3eme lettre) format (nom_prenom) : ");
             // Ajustez la taille selon vos besoins
            char* debutnom = scanString();  // Limiter la saisie pour éviter un dépassement de mémoire
            rechercheAutoAvecAffichage(liste, debutnom);

            break;

        case 2:
            printf("Entrez le nom du contact dont vous voulez afficher les rendez-vous : ");
            char* nomContact = scanString();
            Contact* contactAffiche = rechercheClassique(liste, nomContact); // niveau 0 : les niveaux supérieurs sont des copies

            if (contactAffiche != NULL) {
                afficherRendezVous(contactAffiche);
            }
            else {
                printf("Contact non trouvé.\n");
            }
            break;

        case 3:
            

            printf("Entrez le nom et prénom du contact au format 'nom_prenom': ");
            char* saisie = scanString();

                // Vérifier si la saisie est au format 'nom_prenom'
            int foundUnderscore = 0;
            for (int i = 0; saisie[i] != '\0'; i++) {
                if (saisie[i] == '_') {
                    foundUnderscore = 1;
                   break;
                }
            }

            if (foundUnderscore) {
                Contact* nouveauContact = CreeCellule(saisie);  // Créer le contact seulement si le format est correct
                insererEnOrdre(nouveauContact, liste, 0);
                printf("Le contact a ete cree avec succes.\n");
            }
            else {
                printf("Format incorrect. Assurez-vous que la saisie est au format 'nom_prenom'.\n");
            }
            free(saisie);
            break;
        case 4:
            printf("Entrez le nom du contact pour lequel vous souhaitez creer un rendez-vous : ");
            char* nomContactRdv = scanString();
            Contact* contactRdv = rechercheClassique(liste, nomContactRdv);

            if (contactRdv != NULL) {
                // Contact trouvé, créer un nouveau rendez-vous
                RendezVous* nouveauRdv = CreeRendezVous(); // Assurez-vous d'avoir une fonction pour créer un rendez-vous

                // Ajouter des informations au nouveau rendez-vous
                printf("Entrez la date du rendez-vous (format YYYY-MM-DD) : ");
                scanf("%11s", nouveauRdv->date);

                printf("Entrez l'heure du rendez-vous (format HH:MM) : ");
                scanf("%6s", nouveauRdv->heure);

                printf("Entrez l'objet du rendez-vous : ");
                scanf("%29s", nouveauRdv->ObjetRdv); // un seul mot

                // Ajouter le nouveau rendez-vous à la liste du contact
                insererEnTeteRendezVous(contactRdv, nouveauRdv);

                printf("Le rendez-vous a ete ajoute avec succès.\n");
            }
            else {
                // Contact non trouvé, demander si l'utilisateur souhaite créer un nouveau contact
                printf("Contact non trouve. Voulez-vous creer un nouveau contact ? (Oui/Non) : ");
                char reponse[4]; // Assez grand pour contenir "Oui" ou "Non"
                scanf("%3s", reponse);

                if (strcmp(reponse, "Oui") == 0 || strcmp(reponse, "oui") == 0) {
                    // Créer un nouveau contact
                    Contact* nouveauContact = CreeCellule(nomContactRdv);

                    // Ajouter le nouveau contact à la liste
                    insererEnOrdre(nouveauContact, liste, 0);

                    // Créer un rendez-vous pour le nouveau contact
                    RendezVous* nouveauRdv = CreeRendezVous(); // Assurez-vous d'avoir une fonction pour créer un rendez-vous

                    // Ajouter des informations au nouveau rendez-vous
                    printf("Entrez la date du rendez-vous (format YYYY-MM-DD) : ");
                    scanf("%11s", nouveauRdv->date);

                    printf("Entrez l'heure du rendez-vous (format HH:MM) : ");
                    scanf("%6s", nouveauRdv->heure);

                    printf("Entrez l'objet du rendez-vous : ");
                    scanf("%29s", nouveauRdv->ObjetRdv); // un seul mot

                    // Ajouter le nouveau rendez-vous à la liste du nouveau contact
                    insererEnTeteRendezVous(nouveauContact, nouveauRdv);

                    printf("Le contact et le rendez-vous ont été ajoutés avec succès.\n");
                }
                else {
                    printf("Opération annulée. Le contact n'a pas été trouvé et aucun nouveau contact n'a été créé.\n");
                }
            }
            viderLigne();
            break;

        case 5:
            // Logique pour supprimer un rendez-vous

// Demander le nom du contact
            printf("Entrez le nom du contact pour supprimer un rendez-vous : ");
            char* nomContactASupprimer = scanString();

            // Trouver le contact dans la liste
            Contact* contactASupprimer = rechercheClassique(liste, nomContactASupprimer);

            if (contactASupprimer != NULL) {
                // Afficher les rendez-vous du contact
                afficherRendezVous(contactASupprimer);

                // Demander à l'utilisateur de choisir le rendez-vous à supprimer
                printf("Entrez le numero du rendez-vous a supprimer : ");
                int numeroRdvASupprimer;
                scanf("%d", &numeroRdvASupprimer);
                viderLigne();

                // Supprimer le rendez-vous
                supprimerRendezVous(contactASupprimer, numeroRdvASupprimer);

            }
            else {
                printf("Le contact n'a pas ete trouve.\n");
            }
            break;

        case 6:
            
            sauvegarderContacts(fopen("contacts.txt", "w"), liste);
            break;

        case 7:
            
            
            printf("Entrez le nom et prénom du contact au format 'nom_prenom': ");
            char* saisieC = scanString();

            // Vérifier si la saisie est au format 'nom_prenom'
            int foundUnderscoree = 0;
            for (int i = 0; saisieC[i] != '\0'; i++) {
                if (saisieC[i] == '_') {
                    foundUnderscoree = 1;
                    break;
                }
            }

            if (foundUnderscoree) {
                Contact* nouveauContact = CreeCellule(saisieC);  // Créer le contact seulement si le format est correct
                startTimer();
                insererEnOrdre(nouveauContact, liste, 0); // on insere un contact au niveau 0
                rechercheClassique(liste, nouveauContact->nom_prenom); // recherche niveau 0
                stopTimer();
                printf("Le contact a ete cree avec succes.\n");
                time_lvl0 = getTimeAsString();
                startTimer();
                liste = construireListe_niveaux(4, liste); // on met à jour la liste sur les plusieurs niveaux
                rechercheContact(liste, nouveauContact->nom_prenom); // recherche niveau Haut
                stopTimer();
                time_all_levels = getTimeAsString();
                fprintf(log_file, format, nouveauContact->nom_prenom, time_lvl0, time_all_levels);

                

               
            }
            else {
                printf("Format incorrect. Assurez-vous que la saisie est au format 'nom_prenom'.\n");
            }
            free(saisieC);
            break;

        case 8:
            printf("Sauvegarder les modifications dans contacts.txt avant de quitter ? (o/n) : ");
            char* choixSauvegarde = scanString();
            if (choixSauvegarde[0] == 'o' || choixSauvegarde[0] == 'O') {
                sauvegarderContacts(fopen("contacts.txt", "w"), liste);
                printf("Agenda sauvegardé.\n");
            }
            else {
                printf("Modifications non sauvegardées.\n");
            }
            free(choixSauvegarde);
            free(input);
            fclose(log_file);
            printf("Au revoir !\n");
            return 0;

        default:
            printf("Option invalide.\n");
        }
        fflush(log_file);
    }
    return 0;
}