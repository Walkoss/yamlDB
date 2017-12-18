/*
**  Filename: union.c
**
**  Made by: Alexis PETRILLO on 10/12/2017.
**
**  Description: 
*/

#include "../database/database.h"
#include "../print_color/print_color.h"

int openFilesForInnerJoin(Database *database, Table *table1, Table *table2, char *key, char *key2, void *field,
                          void *condition) {
    FILE *file;
    FILE *file2;
    char *path;
    char *path2;

    if (!database || !table1 || !table2)
        return 1;

    path = getTablePath(database->name, table1->name);
    path2 = getTablePath(database->name, table2->name);
    if (!path || !path2)
        return 1;

    file = fopen(path, "r+");
    file2 = fopen(path2, "r+");
    if (!file || !file2) {
        fprintf(stderr, "%sAn error has occured when removing data in table '%s': "
                "%s\n%s", COLOR_RED, table1->name, strerror(errno), COLOR_RESET);
        free(path);
        free(path2);
        return 1;
    }

    selectFuncInnerJoin(database, file, file2, key, key2, field, condition);
    return 0;
}

long displaySecondFile(Database *database, FILE *file2, char *key2, SelectedData *dataHead) {
    char *keyFile;
    char *valueFile;
    char currentLine[BUFFER_SIZE];
    long positionTmp;
    char **tokens;
    SelectedData *data;

    positionTmp = 0;
    while (fgets(currentLine, BUFFER_SIZE, file2) != NULL) {
        keyFile = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        valueFile = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        data = xmalloc(sizeof(Data), __func__);

        if (!keyFile || !valueFile || !data)
            return 1;

        if (strcmp(currentLine, "\t-\n") == 0)
            return positionTmp;

        tokens = strSplit(currentLine, ':');

        keyFile = tokens[0];
        valueFile = tokens[1];
        keyFile = &keyFile[1]; // Supprime la tabulation
        valueFile = &valueFile[1]; // Supprime le premier espace
        valueFile[strlen(valueFile + 1)] = '\0'; // Supprime l'espace

        data->value = valueFile;
        data->key = keyFile;
        data->next = NULL;
        selectedDataListAppend(&dataHead, data);

        positionTmp = ftell(file2);
    }

    return positionTmp;
}

long isKeyInSecondFile(FILE *file2, char *key2, char *value) {
    char *keyFile;
    char *valueFile;
    char currentLine[BUFFER_SIZE];
    int isData;
    char **tokens;

    isData = 0;
    while (fgets(currentLine, BUFFER_SIZE, file2) != NULL) {
        keyFile = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        valueFile = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!keyFile || !valueFile)
            return 1;

        if (strcmp(currentLine, "\t-\n") == 0) {
            if (isData == 0)
                return 0;
            else
                return ftell(file2);
        }

        tokens = strSplit(currentLine, ':');
        keyFile = tokens[0];
        valueFile = tokens[1];
        keyFile = &keyFile[1]; // Supprime la tabulation
        valueFile = &valueFile[1]; // Supprime le premier espace
        valueFile[strlen(valueFile + 1)] = '\0'; // Supprime l'espace

        /*printf("keyfile : %s\n", keyFile);
        printf("key : %s\n", key2);
        printf("valueFile : %s\n", valueFile);
        printf("value : %s\n", value);*/
        if (strcmp(keyFile, key2) == 0 && strcmp(valueFile, value) == 0 && strcmp(keyFile, "\t-\n") != 0) {
            isData = 1;
        }
    }

    if (isData == 0)
        return 0;
    else
        return ftell(file2);
}

void ExploreSecondFile(Database *database, FILE *file2, char *key2, char *value, SelectedData *dataHead) {
    char currentLine[BUFFER_SIZE];
    long position;
    long positionTmp;

    while (fgets(currentLine, BUFFER_SIZE, file2) != NULL) {
        position = ftell(file2);
        if (strcmp(currentLine, "\t-\n") == 0) {
            positionTmp = isKeyInSecondFile(file2, key2, value);
            fseek(file2, position, SEEK_SET);
            if (positionTmp != 0) {
                fseek(file2, displaySecondFile(database, file2, key2, dataHead), SEEK_SET);
            }
        }
    }
}

int innerJoinWithoutConditionWithoutField(Database *database, FILE *file, FILE *file2, char *key, char *key2) {
    char currentLine[BUFFER_SIZE];
    char *keyFile;
    char *valueFile;
    char **tokens;
    long positionTmp = ftell(file2);
    SelectedData *data;
    SelectedData *dataHead;

    dataHead = NULL;
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        keyFile = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        valueFile = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        data = xmalloc(sizeof(Data), __func__);

        if (!keyFile || !valueFile || !data)
            return 1;

        if (strcmp(currentLine, "\t-\n") != 0) {
            tokens = strSplit(currentLine, ':');
            keyFile = tokens[0];
            valueFile = tokens[1];
            keyFile = &keyFile[1]; // Supprime la tabulation
            valueFile = &valueFile[1]; // Supprime le premier espace
            valueFile[strlen(valueFile + 1)] = '\0'; // Supprime l'espace

            data->value = valueFile;
            data->key = keyFile;
            data->next = NULL;
            selectedDataListAppend(&dataHead, data);
        } else {
            data->value = "-";
            data->key = NULL;
            data->next = NULL;
            selectedDataListAppend(&dataHead, data);
        }

        if (strcmp(keyFile, key) == 0) {
            ExploreSecondFile(database, file2, key2, valueFile, dataHead);
            fseek(file2, positionTmp, SEEK_SET);
        }
    }

    database->selectedData = dataHead;
    return 0;
}

void selectFuncInnerJoin(Database *database, FILE *file, FILE *file2, char *key, char *key2, Field *field,
                         Condition *condition) {
    char currentLine[BUFFER_SIZE];

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        if (strcmp(currentLine, "\t-\n") == 0) {
            if (condition == NULL) {
                if (field == NULL) {
                    innerJoinWithoutConditionWithoutField(database, file, file2, key, key2);
                }
            }
        }
    }

    /* AFFICHAGE DES DATA */
    while (database->selectedData != NULL) {
        if (database->selectedData->key)
            printf("%-20s\t", database->selectedData->value);
        else
            printf("\n");
        database->selectedData = database->selectedData->next;
    }
}
