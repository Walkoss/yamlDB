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
 *
 * @param databaseName
 * @param tableName
 * @return 0 if success, 1 for error
 */
int removeTable(const char *databaseName, const char *tableName) {
    char *path;

    path = xmalloc(sizeof(char) * (strlen(RESOURCES_DIR) + strlen
            (databaseName) + strlen(tableName) + 6), __func__);
    if (!path)
        return 1;

    strcpy(path, RESOURCES_DIR);
    strcat(path, databaseName);
    strcat(path, "/");
    strcat(path, tableName);
    strcat(path, ".yml");

    if (remove(path) == -1) {
        fprintf(stderr, "An error has occured when removing table '%s': "
                "%s\n", tableName, strerror(errno));
        return 1;
    }

    return 0;
}