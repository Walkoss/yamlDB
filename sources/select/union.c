/*
**  Filename: union.c
**
**  Made by: Alexis PETRILLO on 10/12/2017.
**
**  Description: 
*/

#include "../database/database.h"

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
        fprintf(stderr, "An error has occured when removing data in table '%s': "
                "%s\n", table1->name, strerror(errno));
        free(path);
        free(path2);
        return 1;
    }

    selectFuncInnerJoin(file, file2, key, key2, field, condition);
    return 0;
}

long displaySecondFile(FILE *file2, char *key2) {
    char *keyFile;
    char *valueFile;
    char currentLine[BUFFER_SIZE];
    long positionTmp;
    char **tokens;

    positionTmp = 0;
    while (fgets(currentLine, BUFFER_SIZE, file2) != NULL) {
        keyFile = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        valueFile = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!keyFile || !valueFile)
            return 1;

        if (strcmp(currentLine, "-\n") == 0)
            return positionTmp;

        tokens = strSplit(currentLine, ':');

        keyFile = tokens[0];
        valueFile = tokens[1];
        keyFile = &keyFile[1]; // Supprime la tabulation
        valueFile = &valueFile[1]; // Supprime le premier espace

        printf("\t%s: %s", keyFile, valueFile);

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

        if (strcmp(currentLine, "-\n") == 0) {
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

        if (strcmp(keyFile, key2) == 0 && strcmp(valueFile, value) == 0 && strcmp(keyFile, "-\n") != 0) {
            isData = 1;
        }
    }

    if (isData == 0)
        return 0;
    else
        return ftell(file2);
}

void ExploreSecondFile(FILE *file2, char *key2, char *value) {
    char currentLine[BUFFER_SIZE];
    long position;
    long positionTmp;

    while (fgets(currentLine, BUFFER_SIZE, file2) != NULL) {
        position = ftell(file2);
        if (strcmp(currentLine, "-\n") == 0) {
            positionTmp = isKeyInSecondFile(file2, key2, value);
            fseek(file2, position, SEEK_SET);
            if (positionTmp != 0) {
                fseek(file2, displaySecondFile(file2, key2), SEEK_SET);
            }
        }
    }
}

int innerJoinWithoutConditionWithoutField(FILE *file, FILE *file2, char *key, char *key2) {
    char currentLine[BUFFER_SIZE];
    char *keyFile;
    char *valueFile;
    char **tokens;
    long positionTmp = ftell(file2);

    printf("-\n");
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        keyFile = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        valueFile = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!keyFile || !valueFile)
            return 1;

        printf("%s", currentLine);
        tokens = strSplit(currentLine, ':');

        keyFile = tokens[0];
        valueFile = tokens[1];
        keyFile = &keyFile[1]; // Supprime la tabulation
        valueFile = &valueFile[1]; // Supprime le premier espace

        if (strcmp(keyFile, key) == 0 && strcmp(keyFile, "-\n") != 0) {
            ExploreSecondFile(file2, key2, valueFile);
            fseek(file2, positionTmp, SEEK_SET);
        }
    }

    return 0;
}

void selectFuncInnerJoin(FILE *file, FILE *file2, char *key, char *key2, Field *field, Condition *condition) {
    char currentLine[BUFFER_SIZE];

    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        if (strcmp(currentLine, "-\n") == 0) {
            if (condition == NULL) {
                if (field == NULL) {
                    innerJoinWithoutConditionWithoutField(file, file2, key, key2);
                }
            }
        }
    }
}
