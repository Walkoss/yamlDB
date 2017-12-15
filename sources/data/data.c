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

    // TODO: fix this...
    fprintf(file, "-\n");

    while (currentData != NULL) {
        fprintf(file, "\t%s: %s\n", currentData->field->name, currentData->value);
        currentData = currentData->next;
    }

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