/*
**  Filename: database.c
**
**  Made by: Walkoss on 10/11/2017.
**
**  Description: 
*/

#include "database.h"

/**
 * Add a database
 * @param databaseName
 * @return 0 if success, 1 for error
 */
int addDatabase(const char *databaseName) {
    char *path;

    path = xmalloc(sizeof(char) * strlen(RESOURCES_DIR) + strlen
            (databaseName), __func__);
    if (!path)
        return 1;
    strcat(path, RESOURCES_DIR);
    strcat(path, databaseName);
    if (mkdir(path, 0777) == -1) {
        fprintf(stderr, "An error has occured when creating database '%s': "
                "%s\n", databaseName, strerror(errno));
        return 1;
    }
    return 0;
}