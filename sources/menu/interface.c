//
// Created by Iliasse Wahbi on 14/11/2017.
//

#include <ctype.h>
#include "interface.h"

/**
 * Print sentences which depends of result parameter
 * @param result
 */
void isSuccess(int result)
{
    if (!result)
        puts("Action réalisée avec succès.\n");
    else
        puts("Erreur lors de la réalisation de l'action.\n");
}

/**
 * Tells if there is only digit in string
 * @param str
 * @return 1 if success, 0 for error
 */
int isAllDigit(const char* str)
{
    for (int i = 0; i < strlen(str); i++)
        if (!isnumber(str[i]))
            return 0;

    return 1;
}

/**
 * Read user input
 * @param size
 * @return char* if success, NULL for error
 */
char *getUserInput(int size)
{
    char *text;
    int c;
    int i = 0;

    if ((text = xmalloc(sizeof(char) * size + 1, __FUNCTION__)) == NULL)
        return NULL;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (i < size)
        {
            text[i] = (char) c;
            i++;
        }
    }
    text[i] = '\0';

    return text;
}

/**
 * Prints instruction
 * @param sentence, size
 */
void printInstruction(int sentence, int size)
{
    char sentences[6][40] = {"Que voulez-vous faire ?",
                             "1: Ajouter une base de données",
                             "2: Supprimer une base de données",
                             "3: Supprimer une table",
                             "Entrez le nom de la base de données",
                             "Entrez le nom de la table"};

    for (int i = sentence; i < size + sentence; i++)
        printf("%s\n", sentences[i]);
}

/**
 * Calls function that depends of the parameter userChoice
 * @param userChoice
 * @return 0 if success, 1 for error
 */
int choice(long userChoice)
{
    int result;
    char *databaseName;
    char *tableName;
    int (*databaseFuncs[2])(const char *) = {addDatabase, removeDatabase};
    int (*tableFuncs[1])(const char *, const char *) = {removeTable};

    if (userChoice < 0 || userChoice >= 4)
        return 1;
    printInstruction(4, 1);
    databaseName = getUserInput(50);
    if (userChoice < 3)
        result = databaseFuncs[userChoice - 1](databaseName);
    else
    {
        printInstruction(5, 1);
        tableName = getUserInput(50);
        result = tableFuncs[userChoice - 3](databaseName, tableName);
        free(tableName);
    }
    free(databaseName);

    return result;
}

/**
 * Gets user input and begins processes
 * @param userChoice
 * @return 0 if success, 1 for error
 */
int userInterface()
{
    while (1) {
        printInstruction(0, 4);
        char *text = getUserInput(5);
        if (!strcmp(text, "quit"))
        {
            free(text);
            break;
        }
        else if (text[0] != '\0' && isAllDigit(text))
            isSuccess(choice(strtol(text, NULL, 10)));
        free(text);
    }
    return 0;
}