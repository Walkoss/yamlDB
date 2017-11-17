/*
**  Filename: table.c
**
**  Made by: Alexis PETRILLO on 12/11/2017.
**
**  Description: Functions for manage tables
*/

#include "table.h"

/**
 * Init tables structure
 * @param databaseName
 * @return
 *//*
Table *initTables(Database *database) {
    Table *table;
    table = database->tableHead;

    while (table->next != NULL) {
        // TODO: remplir avec les fichiers .yml
        table->name = "tableName";
        // TODO: initFields
        table->fieldHead = NULL;
    }

    return table;
}*/
/*
void freeTables(Database *database) {
    Table *table;

    table = database->tableHead;
    while (table->next != NULL) {
        free(table->name);

        table = table->next;
        // TODO: freeFields
        //free(table);
    }
}*/

/**
 *
 * @param tableName
 * @param database
 * @return
 *//*
int createTable(Database *database, Table *table) {
    char *path;
    FILE *file;

    path = getTablePath(database->name, table->name);
    if (!path)
        return 1;

    file = fopen(path, "w+");
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
}*/

/**
 *
 * @param databaseName
 * @param table
 * @return 0 if success, 1 for error
 *//*
int removeTable(Database *database, Table *table) {
    char *path;

    path = getTablePath(database->name, table->name);
    if (!path)
        return 1;

    if (remove(path) == -1) {
        fprintf(stderr, "An error has occured when removing table '%s': "
                "%s\n", table->name, strerror(errno));

        free(path);
        return 1;
    }

    // TODO: freeTable(database, table) -> freeFields(table)

    free(path);
    return 0;
}*/

/**
 * Based on RESOURCES_DIR and databaseName, this function returns an absolute
 * path to the file
 * @param databaseName
 * @param tableName
 * @return
 *//*
char *getTablePath(const char *databaseName, const char *tableName) {
    char *path;

    path = xmalloc(sizeof(char) * (strlen(RESOURCES_DIR) + strlen
            (databaseName) + strlen(tableName) + 6), __func__);
    if (!path)
        return NULL;

    strcpy(path, RESOURCES_DIR);
    strcat(path, databaseName);
    strcat(path, "/");
    strcat(path, tableName);
    strcat(path, ".yml");

    return path;
}*/