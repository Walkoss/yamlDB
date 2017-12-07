/*
**  Filename: select.c
**
**  Made by: Alexis PETRILLO on 07/12/2017.
**
**  Description: 
*/

#include "../database/database.h"

int displaySingleData(FILE *file, Field *field) {
    char *key;
    char *value;
    char currentLine[BUFFER_SIZE];
    char delim;
    char delim2;
    Field *currentField;
    long position;

    delim = ':'; // Char pour parser les lignes key: value \n
    delim2 = '\n'; // Char pour parser les lignes key: value \n

    currentField = field;
    while (currentField != NULL) {
        position = ftell(file);
        while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
            key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
            value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

            if (!key || !value)
                break;

            if (strcmp(currentLine, "-\n") == 0)
                break;

            key = strtok(currentLine, &delim);
            value = strtok(NULL, &delim2);
            key = &key[1]; // Supprime la tabulation
            value = &value[1]; // Supprime le premier espace

            if (strcmp(key, currentField->name) == 0 && strcmp(key, "-\n") != 0)
                printf("%s: %s\n", key, value);
        }
        fseek(file, position, SEEK_SET);
        currentField = currentField->next;
    }

    fclose(file);
    return 0;
}

int displayAllData(FILE *file, Field *field) {
    char *key;
    char *value;
    char currentLine[BUFFER_SIZE];
    char delim;
    char delim2;

    delim = ':'; // Char pour parser les lignes key: value \n
    delim2 = '\n'; // Char pour parser les lignes key: value \n

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!key || !value)
            return 1;

        if (strcmp(currentLine, "-\n") == 0)
            return 0;

        key = strtok(currentLine, &delim);
        value = strtok(NULL, &delim2);
        key = &key[1]; // Supprime la tabulation
        value = &value[1]; // Supprime le premier espace

        if (strcmp(key, "-\n") != 0)
            printf("%s: %s\n", key, value);
    }

    return 0;
}

int selectData(Database *database, Table *table, Field *field, Condition *condition) {
    FILE *file;
    char *path;
    char currentLine[BUFFER_SIZE];
    long position;
    long positionTmp;

    if (!database || !table || !condition)
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

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        position = ftell(file);
        if (strcmp(currentLine, "-\n") == 0) {
            positionTmp = isConditionFulfilled(file, condition);
            fseek(file, position, SEEK_SET);
            if (positionTmp != 0) {
                if (field->name == NULL)
                    displayAllData(file, field);
                else
                    displaySingleData(file, field);
            }
        }
    }

    fclose(file);

    return 0;
}