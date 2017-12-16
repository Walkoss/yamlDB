/*
**  Filename: select.c
**
**  Made by: Alexis PETRILLO on 07/12/2017.
**
**  Description: 
*/

#include "../database/database.h"

/**
 * Display data to user
 * @param file
 * @param currentField
 * @return position on the file
 */
long displaySingleData(FILE *file, Field *currentField) {
    char currentLine[BUFFER_SIZE];
    char *key;
    char *value;
    char **tokens;
    long positionTmp;

    positionTmp = 0;
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!key || !value)
            return 1;

        if (strcmp(currentLine, "-\n") == 0)
            return positionTmp;

        tokens = strSplit(currentLine, ':');

        key = tokens[0];
        value = tokens[1];
        key = &key[1]; // Supprime la tabulation
        value = &value[1]; // Supprime le premier espace

        if (strcmp(key, currentField->name) == 0 && strcmp(key, "-\n") != 0)
            printf("\t%s: %s", key, value);

        positionTmp = ftell(file);
    }

    return positionTmp;
}

/**
 * Browse data
 * @param file
 * @param field
 * @return position on the file
 */
long BrowseSingleData(FILE *file, Field *field) {
    Field *currentField;
    long position;
    long positionTmp;

    positionTmp = 0;
    currentField = field;
    printf("-\n");
    while (currentField != NULL) {
        position = ftell(file);
        positionTmp = displaySingleData(file, currentField);
        fseek(file, position, SEEK_SET);
        currentField = currentField->next;
    }
    return positionTmp;
}

/**
 * Display all data when have no condition
 * @param file
 * @return 0 if success, 1 for error
 */
int displayAllDataWithoutCondition(FILE *file) {
    char currentLine[BUFFER_SIZE];

    printf("-\n");
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL)
        printf("%s", currentLine);

    return 0;
}

/**
 * Display all data when the * is entered
 * @param file
 * @param field
 * @return position on the file
 */
long displayAllData(FILE *file) {
    char currentLine[BUFFER_SIZE];
    long positionTmp;

    positionTmp = 0;
    printf("-\n");
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        if (strcmp(currentLine, "-\n") == 0)
            return positionTmp;

        printf("%s", currentLine);
        positionTmp = ftell(file);
    }

    return positionTmp;
}

/**
 * select allData if * is entered, single data if not
 * @param file
 * @param field
 * @param condition
 */
void selectMethod(FILE *file, Field *field, Condition *condition) {
    char currentLine[BUFFER_SIZE];
    long position;
    long positionTmp;

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        position = ftell(file);
        if (strcmp(currentLine, "-\n") == 0) {
            if (condition == NULL) {
                if (field == NULL)
                    displayAllDataWithoutCondition(file);
                else
                    fseek(file, BrowseSingleData(file, field), SEEK_SET);
            } else {
                positionTmp = isConditionFulfilled(file, condition);
                fseek(file, position, SEEK_SET);
                if (positionTmp != 0) {
                    if (field == NULL)
                        fseek(file, displayAllData(file), SEEK_SET);
                    else
                        fseek(file, BrowseSingleData(file, field), SEEK_SET);
                }
            }
        }
    }
}

/**
 * Catching error
 * @param database
 * @param table
 * @param field
 * @param condition
 * @return 0 if success, 1 for error
 */
int selectData(Database *database, Table *table, Field *field, Condition *condition) {
    FILE *file;
    char *path;

    if (!database || !table)
        return 1;

    path = getTablePath(database->name, table->name);
    if (!path)
        return 1;

    file = fopen(path, "r+");
    if (!file) {
        fprintf(stderr, "An error has occured when removing data in table '%s': "
                "%s\n", table->name, strerror(errno));
        free(path);
        return 1;
    }

    selectMethod(file, field, condition);
    fclose(file);

    return 0;
}