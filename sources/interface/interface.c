//
// Created by Iliasse Wahbi on 14/11/2017.
//
#include <glib.h>
#include "interface.h"


#define SQL_QUERY_SIZE_BUFFER 1024

const TSentences sentences[] =
        {
                {"Que voulez-vous faire ?"},
                {"1: Ajouter une base de données"},
                {"2: Utiliser une base de données"},
                {"3: Supprimer une base de données"},
                {"4: Créer une table"},
                {"5: Supprimer une table"},
                {"Entrez le nom de la base de données"},
                {"Entrez le nom de la table"},
                {"Entrez le nom du champ à créer"},
                {"Entrez le type de champ\n(1: INT - 2: FLOAT - 3: CHAR - 4: VARCHAR)"},
                {"Ajouter un autre champ ? (N / n pour quitter)"},
                {"Entrez du texte"},
                {"Ce champ existe déjà."},
                {"Action réalisée avec succès."},
                {"Erreur lors de la réalisation de l'action."},
                {NULL}
        };

/**
 * Read user input
 * @return char* if success, NULL for error
 */
char *getUserInput() {
    char *buffer;
    size_t len;

    buffer = xmalloc((SQL_QUERY_SIZE_BUFFER + 1), __func__);
    if (!buffer)
        return NULL;

    if (fgets(buffer, SQL_QUERY_SIZE_BUFFER, stdin) == NULL) {
        free(buffer);
        return NULL;
    }
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    return buffer;
}

char *isSuccessGtk(int result)
{
    if (!result)
        return sentences[13].sentence;
    else
        return sentences[14].sentence;
}