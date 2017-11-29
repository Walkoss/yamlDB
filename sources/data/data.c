/*
**  Filename: data.c
**
**  Made by: Alexis PETRILLO on 25/11/2017.
**
**  Description:
*/

#include "../database/database.h"

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
        fprintf(file, "\t\t%s: %s\n ", currentData->field->name, currentData->value);
        currentData = currentData->next;
    }

    return 0;
}

int removeData(Database *database, Table *table, Condition *condition) {
    FILE *file;
    FILE *filetmp;
    char *path;
    char currentLine[BUFFER_SIZE];
    char *key;
    char *value;
    char *token;
    int isData;

    if (!database || !table || !condition)
        return 1;

    path = getTablePath(database->name, table->name);
    if (!path)
        return 1;

    file = fopen(path, "r+");
    filetmp = fopen(strcat(path, "tmp"), "w+");
    if (!file || !filetmp) {
        fprintf(stderr, "An error has occured when adding data in table '%s': "
                "%s\n", table->name, strerror(errno));
        free(path);
        return 1;
    }

    isData = 0;
    while (fgets(currentLine, BUFFER_SIZE, file) != NULL) {
        key = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);
        value = xmalloc(sizeof(char) * MAX_FIELD_NAME_SIZE, __func__);

        if (!key)
            return 1;

        /*if (isData) {
            fscanf(file, "%s %s", key, value);
            key[strlen(key) - 1]  = '\0'; // To remove the ":"
        }*/

        if (isData) {
            token = strtok(currentLine, ":");

            while (token != NULL) {
                printf("%s", token);

                token = strtok(NULL, ":");
            }
        } else {
            fputs(currentLine, filetmp);
        }

        if (strcmp(currentLine, "data:\n") == 0) {
            isData = 1;
        }

        /*if (isData == 1 && strcmp(key, condition->key) == 0 && strcmp(value, condition->value) == 0) {
            printf("ok\n");
        }*/
    }

    return 0;
}
