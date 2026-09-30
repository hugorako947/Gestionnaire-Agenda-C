#include "saisie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* scanString(void) {
    char buffer[BUFFER_SIZE];
    char* result = NULL;
    size_t len = 0;

    // Saisie de la chaîne de caractères
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        exit(EXIT_SUCCESS); // fin de saisie (Ctrl+Z / Ctrl+D) : on quitte proprement
    }

    // Suppression du caractère de nouvelle ligne ajouté par fgets
    buffer[strcspn(buffer, "\n")] = 0;

    // Allouer la mémoire nécessaire et copier la chaîne
    len = strlen(buffer);
    result = (char*)malloc((len + 1) * sizeof(char));
    if (result != NULL) {
        strcpy(result, buffer);
    }
    else {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    return result;
}