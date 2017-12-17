/*
**  Filename: data.c
**
**  Made by: Alexis PETRILLO on 25/11/2017.
**
**  Description:
*/

#include "../database/database.h"
#include "../print_color/print_color.h"

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

    fprintf(file, "\t-\n");

    while (currentData != NULL) {
        fprintf(file, "\t%s: %s\n", currentData->field->name, currentData->value);
        currentData = currentData->next;
    }

    fclose(file);

    return 0;
}

Data *dataListLast(Data *node) {
    while (node->next != NULL)
        node = node->next;
    return (node);
}

void dataListAppend(Data **node, Data *newNode) {
    if (*node != NULL)
        dataListLast(*node)->next = newNode;
    else
        *node = newNode;
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
    char *prevLine;

    prevLine = "";
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        printf("PREVLINE = %s\nCURRENTLINE = %s\n", prevLine, currentLine);
        position = ftell(file);

        if (strcmp(currentLine, "\t-\n") == 0) {
            if (condition != NULL)
                positionTmp = isConditionFulfilled(file, condition);
            else {
                fseek(fileTmp, 0, SEEK_END);
                positionTmp = ftell(fileTmp);
                fclose(fileTmp);
            }

            if (positionTmp == 0) {// Si ne répond pas à la condition where on replace le pointeur
                fseek(file, position, SEEK_SET);
            } else {
                fseek(file, positionTmp, SEEK_SET);
            }
        }
        if (strcmp(prevLine, "data:\n") != 0)
            fputs(currentLine, fileTmp);
        prevLine = strdup(currentLine);
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

    if (!database || !table)
        return 1;

    path = getTablePath(database->name, table->name);
    pathTmp = strcat(getTablePath(database->name, table->name), "tmp");
    if (!path || !pathTmp)
        return 1;

    file = fopen(path, "r+");
    fileTmp = fopen(pathTmp, "w+");
    if (!file || !fileTmp) {
        fprintf(stderr, "%sAn error has occured when removing data in table '%s': "
                "%s%s\n", COLOR_RED, table->name, strerror(errno), COLOR_RESET);
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
    char **tokens;

    isData = 0;
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!key || !value)
            return 1;

        if (strcmp(currentLine, "\t-\n") == 0) {
            if (isData == 0)
                return 0;
            else
                return ftell(file);
        }
        tokens = strSplit(currentLine, ':');

        key = tokens[0];
        value = tokens[1];
        key = &key[1]; // Supprime la tabulation
        value = &value[1]; // Supprime le premier espace
        value[strlen(value) - 1] = '\0'; // Supprime le /n

        if (strcmp(value, condition->value) == 0 && strcmp(key, condition->key) == 0)
            isData = 1;
    }

    if (isData == 0)
        return 0;
    else
        return ftell(file);
}


/**
 * Update data on file
 * @param file
 * @param fileTmp
 * @param data
 * @return 0 if success, 1 for error
 */
long updateDataOnFile(FILE *file, FILE *fileTmp, Data *data) {
    char *key;
    char *value;
    char currentLine[BUFFER_SIZE];
    char **tokens;
    long positionTmp;

    positionTmp = 0;
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!key || !value)
            return 1;

        if (strcmp(currentLine, "\t-\n") == 0)
            return positionTmp;

        tokens = strSplit(currentLine, ':');

        key = tokens[0];
        value = tokens[1];
        key = &key[1]; // Supprime la tabulation
        value = &value[1]; // Supprime le premier espace

        fputs("\t", fileTmp);
        fputs(key, fileTmp);
        fputs(": ", fileTmp);

        if (strcmp(key, data->field->name) == 0 && strcmp(key, "\t-\n") != 0) {
            fputs(data->value, fileTmp);
            fputs("\n", fileTmp);
        } else if (strcmp(key, data->field->name) != 0 && strcmp(key, "\t-\n") != 0)
            fputs(value, fileTmp);

        positionTmp = ftell(file);
    }

    return 0;
}

int updateEveryData(FILE *file, FILE *fileTmp, Data *data) {
    char *key;
    char *value;
    char currentLine[BUFFER_SIZE];
    char **tokens;

    fputs("\t-\n", fileTmp);
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!key || !value)
            return 1;

        if (strcmp(currentLine, "\t-\n") != 0) {
            tokens = strSplit(currentLine, ':');

            key = tokens[0];
            value = tokens[1];
            key = &key[1]; // Supprime la tabulation
            value = &value[1]; // Supprime le premier espace

            if (strcmp(key, data->field->name) == 0 && strcmp(key, "\t-\n") != 0) {
                fputs("\t", fileTmp);
                fputs(key, fileTmp);
                fputs(": ", fileTmp);
                fputs(data->value, fileTmp);
                fputs("\n", fileTmp);
            } else {
                fputs("\t", fileTmp);
                fputs(key, fileTmp);
                fputs(": ", fileTmp);
                fputs(value, fileTmp);
            }
        } else
            fputs("\t-\n", fileTmp);
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

        if (strcmp(currentLine, "\t-\n") == 0) {
            if (condition == NULL) {
                updateEveryData(file, fileTmp, data);
                break;
            } else {
                positionTmp = isConditionFulfilled(file, condition);
                fseek(file, position, SEEK_SET);
                if (positionTmp != 0) {
                    updateDataOnFile(file, fileTmp, data);
                }
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

    if (!database || !table)
        return 1;

    path = getTablePath(database->name, table->name);
    pathTmp = strcat(getTablePath(database->name, table->name), "tmp");
    if (!path || !pathTmp)
        return 1;

    file = fopen(path, "r+");
    fileTmp = fopen(pathTmp, "w+");
    if (!file || !fileTmp) {
        fprintf(stderr, "%sAn error has occured when removing data in table '%s': "
                "%s\n%s", COLOR_RED, table->name, strerror(errno), COLOR_RESET);
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