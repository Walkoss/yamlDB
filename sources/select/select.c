/*
**  Filename: select.c
**
**  Made by: Alexis PETRILLO on 07/12/2017.
**
**  Description: 
*/

#include "../database/database.h"

SelectedData *selectedDataListLast(SelectedData *node) {
    while (node->next != NULL)
        node = node->next;
    return (node);
}

void selectedDataListAppend(SelectedData **node, SelectedData *newNode) {
    if (*node != NULL)
        selectedDataListLast(*node)->next = newNode;
    else
        *node = newNode;
}

/**
 * Display data to user
 * @param file
 * @param currentField
 * @return position on the file
 */
long displaySingleData(FILE *file, Field *currentField, Database *database) {
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

        if (strcmp(key, currentField->name) == 0 && strcmp(key, "-\n") != 0) {
            printf("\t%s: %s", key, value);
        }

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
long BrowseSingleData(FILE *file, Field *field, Database *database) {
    Field *currentField;
    long position;
    long positionTmp;

    positionTmp = 0;
    currentField = field;
    printf("-\n");
    while (currentField != NULL) {
        position = ftell(file);
        positionTmp = displaySingleData(file, currentField, database);
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
int displayAllDataWithoutCondition(FILE *file, Database *database) {
    char currentLine[BUFFER_SIZE];
    char **tokens;
    SelectedData *dataHead;
    dataHead = NULL;
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        char *key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        char *value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        SelectedData *data = xmalloc(sizeof(SelectedData), __func__);

        if (!key || !value || !data)
            return 1;

        data->value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        data->key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (strcmp(currentLine, "-\n") != 0) {

            tokens = strSplit(currentLine, ':');
            key = tokens[0];
            value = tokens[1];

            key = &key[1]; // Supprime la tabulation
            value = &value[1]; // Supprime le premier espace
            value[strlen(value+1)] = '\0'; // Supprime l'espace

            strcpy(data->value, value);
            strcpy(data->key, key);
            data->next = NULL;
            selectedDataListAppend(&dataHead, data);
        }
        else {
            strcpy(data->value, "-");
            data->key = NULL;
            data->next = NULL;
            selectedDataListAppend(&dataHead, data);
        }
    }
    database->selectedData = dataHead;

    /*while (database->selectedData != NULL) {
        if (database->selectedData->key)
            printf("%s: %s\n", database->selectedData->key, database->selectedData->value);
        else
            printf("-\n");
        database->selectedData = database->selectedData->next;
    }*/
    return 0;
}

/**
 * Display all data when the * is entered
 * @param file
 * @param field
 * @return position on the file
 */
long displayAllData(FILE *file, Database *database) {
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
void selectMethod(FILE *file, Field *field, Condition *condition, Database *database) {
    char currentLine[BUFFER_SIZE];
    long position;
    long positionTmp;

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        position = ftell(file);
        if (strcmp(currentLine, "-\n") == 0) {
            if (condition == NULL) {
                if (field == NULL)
                    displayAllDataWithoutCondition(file, database);
                else
                    fseek(file, BrowseSingleData(file, field,  database), SEEK_SET);
            } else {
                positionTmp = isConditionFulfilled(file, condition);
                fseek(file, position, SEEK_SET);
                if (positionTmp != 0) {
                    if (field == NULL)
                        fseek(file, displayAllData(file,  database), SEEK_SET);
                    else
                        fseek(file, BrowseSingleData(file, field,  database), SEEK_SET);
                }
            }
        }
    }

    /*while (database->selectedData != NULL) {
        if (database->selectedData->key)
            printf("%s: %s\n", database->selectedData->key, database->selectedData->value);
        else
            printf("-\n");
        database->selectedData = database->selectedData->next;
    }*/
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

    selectMethod(file, field, condition, database);
    fclose(file);

    return 0;
}