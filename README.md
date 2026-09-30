# Agenda de contacts en C — liste chaînée à niveaux

Application en ligne de commande, écrite en C, qui gère un carnet de contacts et leurs rendez-vous. Les contacts sont rangés dans une **liste chaînée à plusieurs niveaux**, une structure inspirée des *skip lists* qui sert à accélérer la recherche. Le programme permet aussi de mesurer les temps d'insertion et de recherche.

> Projet réalisé en [année] dans le cadre de [cours / formation], [seul / en binôme avec …]. *(à compléter)*

## Fonctionnalités

- Chargement automatique des contacts et de leurs rendez-vous depuis `contacts.txt` au démarrage (250 contacts fictifs fournis)
- Recherche par autocomplétion à partir des 3 premières lettres
- Affichage, ajout et suppression des rendez-vous d'un contact
- Création de nouveaux contacts au format `nom_prenom`
- Sauvegarde de l'agenda dans `contacts.txt`
- Mesure des temps de calcul : insertion + recherche sur le niveau 0 seul, comparée à la reconstruction de la liste sur 4 niveaux + recherche. Les résultats sont ajoutés à `log.txt`
- Option pour quitter, avec proposition de sauvegarde avant la fermeture

## Aperçu

```
[1]. Rechercher un contact           [2]. Afficher les rendez-vous d'un contact
[3]. Creer un contact                [4]. Creer un rendez-vous pour un contact
[5]. Supprimer un rendez-vous        [6]. Sauvegarder le fichier de tous les rendez-vous
[7]. Fournir les temps de calcul pour une insertion de nouveau contact
[8]. Quitter

Choisissez une option : 1
Entrez le nom partiel pour la recherche (a partir de la 3eme lettre) format (nom_prenom) : azar
Résultats de la recherche pour : azar
Nom : azard_philicia

Choisissez une option : 2
Entrez le nom du contact dont vous voulez afficher les rendez-vous : azard_philicia
Rendez-vous pour azard_philicia :
[1] Date : 2024-03-12, Heure : 14:30, Objet : Entretien
```

## La structure de données : une liste à niveaux

Chaque contact (`Contact`) contient son nom, un pointeur vers le contact suivant et une liste chaînée de rendez-vous (`RendezVous`). La liste principale compte 4 niveaux :

| Niveau | Contenu |
|--------|---------|
| 0 | Tous les contacts, triés par ordre alphabétique |
| 1 | Un contact « repère » par groupe de noms partageant les mêmes 3 premières lettres |
| 2 | Un repère à chaque changement de 2e lettre |
| 3 | Un repère à chaque changement de 1re lettre |

Les niveaux supérieurs sont reconstruits à partir du niveau 0 (`construireListe_niveaux`). La recherche multi-niveaux commence par le niveau le plus haut, qui contient peu d'éléments, puis descend vers le niveau 0.

## Organisation du code

| Fichier | Rôle |
|---------|------|
| `main.c` | Menu et boucle principale |
| `cellule.c` / `cellule.h` | Structures `Contact` et `RendezVous`, création d'un contact, comparaison des noms selon le niveau |
| `liste.c` / `liste.h` | Liste à niveaux : insertion triée, construction des niveaux, recherches, gestion des rendez-vous |
| `Fichier.c` / `Fichier.h` | Chargement et sauvegarde de `contacts.txt`, génération de contacts aléatoires, normalisation des noms |
| `saisie.c` / `saisie.h` | Lecture d'une ligne au clavier avec allocation dynamique |
| `timer.c` / `timer.h` | Chronomètre utilisé pour les mesures de temps |
| `contacts.txt` | Jeu de données (contacts fictifs) |
| `AgendaFinal.cbp` | Projet Code::Blocks |

## Compiler et lancer

Il faut un compilateur C : GCC (MinGW sous Windows), ou gcc / clang sous Linux et macOS. Lance le programme **depuis le dossier du projet**, car il lit `contacts.txt` dans le dossier courant.

**Windows (PowerShell)**

```
gcc -Wall -o agenda.exe main.c cellule.c liste.c Fichier.c saisie.c timer.c
.\agenda.exe
```

**Linux / macOS / WSL**

```
gcc -Wall -o agenda main.c cellule.c liste.c Fichier.c saisie.c timer.c -lm
./agenda
```

**Code::Blocks** : ouvrir `AgendaFinal.cbp`, puis *Build and run* (F9).

Pour quitter, choisis l'option 8 : le programme propose alors de sauvegarder l'agenda avant de se fermer.

## Format du fichier `contacts.txt`

Un contact par ligne. Ses rendez-vous, s'il en a, figurent sur les lignes suivantes :

```
azard_philicia
Rendez-vous : Date : 2024-03-12, Heure : 14:30, Objet : Entretien
babootarie_laurye
```

## Compétences mises en œuvre

- Langage C : pointeurs, allocation et libération dynamique de la mémoire, structures
- Listes chaînées triées et structure de données à plusieurs niveaux
- Lecture et écriture de fichiers texte
- Programmation modulaire (séparation en fichiers `.c` / `.h`)
- Mesure et comparaison de performances

## Pistes d'amélioration

- Faire descendre la recherche d'un niveau à l'autre à partir du dernier repère, au lieu de reparcourir chaque niveau depuis sa tête, pour tirer pleinement parti de la structure.
- Faire pointer tous les niveaux vers les mêmes contacts (un tableau `suivants[niveau]` dans chaque contact) au lieu de copier les contacts dans les niveaux supérieurs.
- Vérifier le format des dates et des heures saisies.
- Libérer toute la mémoire à la fermeture du programme.

## Auteur

[Prénom Nom] — [lien LinkedIn] *(à compléter)*
