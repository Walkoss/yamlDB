/*
**  Filename: select.c
**
**  Made by: Alexis PETRILLO on 07/12/2017.
**
**  Description: 
*/

#include "../database/database.h"
#include "../print_color/print_color.h"
#include "../hash_map/hash_map.h"

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
void displaySingleData(FILE *file, Field *currentField, Database *database, Condition *condition) {
    char currentLine[BUFFER_SIZE];
    char *key;
    char *value;
    char **tokens;
    Field *fieldCopy;
    SelectedData *data;
    SelectedData *dataHead;
    HashMap *hashMap;
    int conditionFulfilled;

    dataHead = NULL;
    hashMap = hashMapInit(5);
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!key || !value)
            return;

        if (strcmp(currentLine, "\t-\n") == 0) {
            conditionFulfilled = 0;
            if (condition) {
                if (!strcmp(hashMapGet(hashMap, condition->key), condition->value)) {
                    fieldCopy = currentField;
                    while (fieldCopy != NULL) {
                        data = xmalloc(sizeof(Data), __func__);
                        data->value = (char *) hashMapGet(hashMap, fieldCopy->name);
                        data->key = fieldCopy->name;
                        data->next = NULL;
                        selectedDataListAppend(&dataHead, data);
                        fieldCopy = fieldCopy->next;
                        conditionFulfilled = 1;
                    }
                }
            } else {
                fieldCopy = currentField;
                while (fieldCopy != NULL) {
                    data = xmalloc(sizeof(Data), __func__);
                    data->value = (char *) hashMapGet(hashMap, fieldCopy->name);
                    data->key = fieldCopy->name;
                    data->next = NULL;
                    selectedDataListAppend(&dataHead, data);
                    fieldCopy = fieldCopy->next;
                }
            }

            if (conditionFulfilled || !condition) {
                data = xmalloc(sizeof(Data), __func__);
                data->value = "-";
                data->key = NULL;
                data->next = NULL;
                selectedDataListAppend(&dataHead, data);
            }
        } else {
            tokens = strSplit(currentLine, ':');
            key = tokens[0];
            value = tokens[1];
            key = &key[1]; // Supprime la tabulation
            value = &value[1]; // Supprime le premier espace
            value[strlen(value + 1)] = '\0'; // Supprime l'espace
            hashMapPut(hashMap, key, strdup(value));
        }
    }

    if (condition) {
        if (!strcmp(hashMapGet(hashMap, condition->key), condition->value)) {
            fieldCopy = currentField;
            while (fieldCopy != NULL) {
                data = xmalloc(sizeof(Data), __func__);
                data->value = (char *) hashMapGet(hashMap, fieldCopy->name);
                data->key = fieldCopy->name;
                data->next = NULL;
                selectedDataListAppend(&dataHead, data);
                fieldCopy = fieldCopy->next;
            }
        }
    } else {
        fieldCopy = currentField;
        while (fieldCopy != NULL) {
            data = xmalloc(sizeof(Data), __func__);
            data->value = (char *) hashMapGet(hashMap, fieldCopy->name);
            data->key = fieldCopy->name;
            data->next = NULL;
            selectedDataListAppend(&dataHead, data);
            fieldCopy = fieldCopy->next;
        }
    }

    database->selectedData = dataHead;
}

/**
 * Browse data
 * @param file
 * @param field
 * @return position on the file
 */
long BrowseSingleData(FILE *file, Field *field, Database *database) {
//    long position;
//    long positionTmp;
//
//    position = ftell(file);
//    positionTmp = displaySingleData(file, field, database);
//    fseek(file, position, SEEK_SET);

    return 4;
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

        if (strcmp(currentLine, "\t-\n") != 0) {
            tokens = strSplit(currentLine, ':');
            key = tokens[0];
            value = tokens[1];
            key = &key[1]; // Supprime la tabulation
            value = &value[1]; // Supprime le premier espace
            value[strlen(value + 1)] = '\0'; // Supprime l'espace

            strcpy(data->value, value);
            strcpy(data->key, key);
            data->next = NULL;
            selectedDataListAppend(&dataHead, data);
        } else {
            data->value = "-";
            data->key = NULL;
            data->next = NULL;
            selectedDataListAppend(&dataHead, data);
        }
    }

    database->selectedData = dataHead;
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
    char *key;
    char *value;
    char **tokens;
    SelectedData *data;
    SelectedData *dataHead;

    dataHead = NULL;
    positionTmp = 0;
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        data = xmalloc(sizeof(Data), __func__);
        if (strcmp(currentLine, "\t-\n") == 0) {
            /*data->value = "-";
            data->key = NULL;
            data->next = NULL;
            selectedDataListAppend(&dataHead, data);*/
            database->selectedData = dataHead;
            return positionTmp;
        }

        if (!key || !value || !data)
            return 1;

        if (strcmp(currentLine, "\t-\n") != 0) {
            tokens = strSplit(currentLine, ':');
            key = tokens[0];
            value = tokens[1];
            key = &key[1]; // Supprime la tabulation
            value = &value[1]; // Supprime le premier espace
            value[strlen(value + 1)] = '\0'; // Supprime l'espace

            data->value = value;
            data->key = key;
            data->next = NULL;
            selectedDataListAppend(&dataHead, data);
        }

        positionTmp = ftell(file);
    }

    database->selectedData = dataHead;
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
    Field *fieldCopy;

    // To consume meta data
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        position = ftell(file);
        if (strcmp(currentLine, "\t-\n") == 0) {
            break;
        }
    }

    displaySingleData(file, field, database, condition);
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
        sprintf(error, "%sAn error has occured when removing data in table '%s': "
                "%s\n%s", COLOR_RED, table->name, strerror(errno), COLOR_RESET);
        free(path);
        return 1;
    }

    selectMethod(file, field, condition, database);
    fclose(file);

    return 0;
}