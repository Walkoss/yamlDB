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
        if (currentData->field->type == PK || currentData->field->type == INT)
            fprintf(file, "\t%s: %d\n ", currentData->field->name, currentData->iValue);
        else if (currentData->field->type == FLOAT)
            fprintf(file, "\t%s: %f\n ", currentData->field->name, currentData->fValue);
        else if (currentData->field->type == CHAR)
            fprintf(file, "\t%s: %c\n ", currentData->field->name, currentData->cValue);
        else if (currentData->field->type == VARCHAR)
            fprintf(file, "\t%s: %s\n ", currentData->field->name, currentData->sValue);

        currentData = currentData->next;
    }

    return 0;
}
