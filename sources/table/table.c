/*
**  Filename: table.c
**
**  Made by: Alexis PETRILLO on 12/11/2017.
**
**  Description: Functions for manage tables
*/

#include "../database/database.h"
#include "table.h"

/**
 * Init tables structure
 * @param databaseName
 * @return
 */
Table *initTables(char *tableName) {
    Table *table;
    table = xmalloc(sizeof(Table), __func__);

    while (table->next != NULL) {
        table->name = tableName;
        // TODO: initFields
        table->fieldHead = NULL;
    }

    return table;
}

void freeTables(Table *table) {
    while (table->next != NULL) {
        free(table->name);

        table = table->next;
        // TODO: freeFields
        //free(table);
    }
}

/**
 *
 * @param table
 * @param file
 * @return 0 if success
 */
int addFields(Table *table, FILE *file) {
    Field *field;

    field = table->fieldHead;
    fprintf(file, "fields:\n");

    while (field != NULL) {
        fprintf(file, "\t%s: %d\n", field->name, field->type);
        field = field->next;
    }

    return 0;
}

/**
 *
 * @param databaseName
 * @param table
 * @return 0 if success, 1 for error
 */
int createTable(const char *databaseName, Table *table) {
    char *path;
    FILE *file;

    path = xmalloc(sizeof(char) * strlen(RESOURCES_DIR) + strlen
            (table->name), __func__);
    if (!path)
        return 1;

    strcpy(path, RESOURCES_DIR);
    strcat(path, databaseName);
    strcat(path, "/");
    strcat(path, table->name);
    strcat(path, ".yml");
    file = fopen(path, "w");

    if (!file) {
        fprintf(stderr, "An error has occured when creating table '%s': "
                "%s\n", table->name, strerror(errno));
        free(path);
        return 1;
    }

    addFields(table, file);
    fclose(file);
    free(path);

    return 0;
}

/**
 *
 * @param databaseName
 * @param table
 * @return 0 if success, 1 for error
 */
int removeTable(const char *databaseName, Table *table) {
    char *path;

    path = xmalloc(sizeof(char) * (strlen(RESOURCES_DIR) + strlen
            (databaseName) + strlen(table->name) + 6), __func__);
    if (!path)
        return 1;

    strcpy(path, RESOURCES_DIR);
    strcat(path, databaseName);
    strcat(path, "/");
    strcat(path, table->name);
    strcat(path, ".yml");

    if (remove(path) == -1) {
        fprintf(stderr, "An error has occured when removing table '%s': "
                "%s\n", table->name, strerror(errno));

        free(path);
        return 1;
    }

    free(path);
    return 0;
}
