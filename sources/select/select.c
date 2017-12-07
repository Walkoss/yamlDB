/*
**  Filename: select.c
**
**  Made by: Alexis PETRILLO on 07/12/2017.
**
**  Description: 
*/

#include "../database/database.h"

int displayData(FILE *file, Field *field) {
    printf("test");
    if (field->name == NULL) {
        printf("test");
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
                displayData(file, field);
            }
        }
    }

    fclose(file);

    return 0;
}