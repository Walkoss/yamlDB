/*
**  Filename: data.c
**
**  Made by: Alexis PETRILLO on 25/11/2017.
**
**  Description:
*/

#include "../database/database.h"

/**
 * Add data on file
 * @param database
 * @param table
 * @param data
 * @return 1 for error, 0 if success
 */
int addData(Database *database, Table *table, Data *data) {
    FILE *file;
    char *path;
    Data *currentData;

    if (!database || !table || !data)
        return 1;

    path = getTablePath(database->name, table->name);
    if (!path)
        return 1;

    file = fopen(path, "a+");
    if (!file) {
        fprintf(stderr, "An error has occured when adding data in table '%s': "
                "%s\n", table->name, strerror(errno));
        free(path);
        return 1;
    }

    currentData = data;
    fprintf(file, "-\n");

    while (currentData != NULL) {
        fprintf(file, "\t%s: %s\n", currentData->field->name, currentData->value);
        currentData = currentData->next;
    }

    return 0;
}
/**
 * remove the data from file
 * @param file
 * @param condition
 * @return a position if a data has to be removed, 0 if not
 */
long removeDataFromFile(FILE *file, Condition *condition) {
    char *key;
    char *value;
    char currentLine[BUFFER_SIZE];
    int isData;
    char delim;
    char delim2;

    isData = 0;
    delim = ':'; // Char pour parser les lignes key: value \n
    delim2 = '\n'; // Char pour parser les lignes key: value \n

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!key || !value)
            return 1;

        if (strcmp(currentLine, "-\n") == 0) {
            if (isData == 0)
                return 0;
            else
                return ftell(file);
        }

        key = strtok(currentLine, &delim);
        value = strtok(NULL, &delim2);
        key = &key[1]; // Supprime la tabulation
        value = &value[1]; // Supprime le premier espace

        
        if (strcmp(value, condition->value) == 0 && strcmp(key, condition->key) == 0 && strcmp(key, "-\n") != 0)
            isData = 1;
    }

    if (isData == 0)
        return 0;
    else
        return ftell(file);
}

/**
 * remove data
 * @param database
 * @param table
 * @param condition
 * @return 0 for success, 0 if error
 */
int removeData(Database *database, Table *table, Condition *condition) {
    FILE *file;
    FILE *fileTmp;
    char *path;
    char *pathTmp;
    char currentLine[BUFFER_SIZE];
    long position;
    long positionTmp;

    if (!database || !table || !condition)
        return 1;

    path = getTablePath(database->name, table->name);
    pathTmp = strcat(getTablePath(database->name, table->name), "tmp");
    if (!path || !pathTmp)
        return 1;

    file = fopen(path, "r+");
    fileTmp = fopen(pathTmp, "w+");
    if (!file || !fileTmp) {
        fprintf(stderr, "An error has occured when removing data in table '%s': "
                "%s\n", table->name, strerror(errno));
        free(path);
        free(pathTmp);
        return 1;
    }

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        position = ftell(file);
        if (strcmp(currentLine, "-\n") == 0) {
            positionTmp = removeDataFromFile(file, condition);
            if (positionTmp == 0) // Si ne répond pas à la condition where on replace le pointeur
                fseek(file, position, SEEK_SET);
            else
                fseek(file, positionTmp, SEEK_SET);
        }
        fputs(currentLine, fileTmp);
    }

    fclose(file);
    fclose(fileTmp);
    remove(path);
    rename(pathTmp, path);
    remove(pathTmp);

    return 0;
}
