//
// Created by Iliasse Wahbi on 14/11/2017.
//
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
    while ((c = getchar()) != '\n' && c != EOF) {
        if (i < size) {
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
    char sentences[8][40] = {"Que voulez-vous faire ?",
                             "1: Ajouter une base de données",
                             "2: Utiliser une base de données",
                             "3: Supprimer une base de données",
                             "4: Créer une table",
                             "5: Supprimer une table",
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
Database *choice(long userChoice, Database *database)
{
    int result;
    char *databaseName;
    char *tableName;
    int (*databaseFuncs[3])(Database*) = {createDatabase, useDatabase, dropDatabase};
    int (*tableFuncs[2])(Database*, Table*) = {createTable, dropTable};



    if (userChoice < 0 || userChoice >= 6)
        return NULL;
    printInstruction(6, 1);

    if (userChoice < 4) {
        databaseName = getUserInput(50);
        database = initDatabase(databaseName);
        result = databaseFuncs[userChoice - 1](database);
        free(databaseName);
    }
    else
    {
        Table *table1 = xmalloc(sizeof(Table), __func__);
        Field *field1 = xmalloc(sizeof(Field), __func__);
        Field *field2 = xmalloc(sizeof(Field), __func__);

        table1->name = "test";
        table1->pk = 0;
        table1->fieldHead = field1;
        table1->next = NULL;

        field1->name = "field1";
        field1->type = VARCHAR;
        field1->next = field2;

        field2->name = "field2";
        field2->type = INT;
        field2->next = NULL;

        printInstruction(7, 1);
        result = tableFuncs[userChoice - 4](database, table1);
        //free(tableName);
    }


    //return result;
    return database;
}

/**
 * Gets user input and begins processes
 * @param userChoice
 * @return 0 if success, 1 for error
 */
int userInterface()
{
    Database *database = NULL;

    while (1) {
        printInstruction(0, 6);
        char *text = getUserInput(5);
        if (!strcmp(text, "quit"))
        {
            free(text);
            break;
        }
        else if (text[0] != '\0' && isAllDigit(text))
            database = choice(strtol(text, NULL, 10), database);

        printf("DEBUG : %s", database->name);
        free(text);
    }
    return 0;
}