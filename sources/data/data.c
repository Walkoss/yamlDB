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
 * @return 0 if success, 1 for error
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
 * Update data on file
 * @param file
 * @param fileTmp
 * @param data
 * @return 0 if success, 1 for error
 */
int updateDataOnFile(FILE *file, FILE *fileTmp, Data *data) {
    char *key;
    char *value;
    char currentLine[BUFFER_SIZE];
    char delim;
    char delim2;

    delim = ':'; // Char pour parser les lignes key: value \n
    delim2 = '\n'; // Char pour parser les lignes key: value \n

    fputs("-\n", fileTmp);
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

        fputs("\t", fileTmp);
        fputs(key, fileTmp);
        fputs(": ", fileTmp);

        if (strcmp(key, data->field->name) == 0 && strcmp(key, "-\n") != 0)
            fputs(data->value, fileTmp);
        else if (strcmp(key, data->field->name) != 0 && strcmp(key, "-\n") != 0)
            fputs(value, fileTmp);

        fputs("\n", fileTmp);
    }

    return 0;
}


/**
 * Update data
 * @param file
 * @param fileTmp
 * @param data
 * @param condition
 * @return
 */
void updateData(FILE *file, FILE *fileTmp, Data *data, Condition *condition) {
    char currentLine[BUFFER_SIZE];
    long position;
    long positionTmp;

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        position = ftell(file);
        if (strcmp(currentLine, "-\n") == 0) {
            positionTmp = isConditionFulfilled(file, condition);
            fseek(file, position, SEEK_SET);
            if (positionTmp != 0) {
                updateDataOnFile(file, fileTmp, data);
            }
        }
        fputs(currentLine, fileTmp);
    }
}


/**
 * Open Files before update data
 * @param database
 * @param table
 * @param data
 * @param condition
 * @return
 */
int openFilesForUpdating(Database *database, Table *table, Data *data, Condition *condition) {
    FILE *file;
    FILE *fileTmp;
    char *path;
    char *pathTmp;

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

    updateData(file, fileTmp, data, condition);

    fclose(file);
    fclose(fileTmp);
    remove(path);
    rename(pathTmp, path);
    remove(pathTmp);

    return 0;
}

/**
 * Remove data on file
 * @param database
 * @param table
 * @param condition
 * @return 0 if success, 1 for error
 */
void removeData(FILE *file, FILE *fileTmp, Condition *condition) {
    char currentLine[BUFFER_SIZE];
    long position;
    long positionTmp;

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        position = ftell(file);
        if (strcmp(currentLine, "-\n") == 0) {
            positionTmp = isConditionFulfilled(file, condition);
            if (positionTmp == 0) // Si ne répond pas à la condition where on replace le pointeur
                fseek(file, position, SEEK_SET);
            else
                fseek(file, positionTmp, SEEK_SET);
        }
        fputs(currentLine, fileTmp);
    }
}


/**
 * Open Files before remove data
 * @param database
 * @param table
 * @param data
 * @param condition
 * @return
 */
int openFilesForRemoving(Database *database, Table *table, Condition *condition) {
    FILE *file;
    FILE *fileTmp;
    char *path;
    char *pathTmp;

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

    removeData(file, fileTmp, condition);

    fclose(file);
    fclose(fileTmp);
    remove(path);
    rename(pathTmp, path);
    remove(pathTmp);

    return 0;
}

/**
 * Check if the condition is fulfilled
 * @param file
 * @param condition
 * @return 0 if not fulfilled, 1 for error, position is fulfilled
 */
long isConditionFulfilled(FILE *file, Condition *condition) {
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
