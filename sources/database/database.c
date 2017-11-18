/*
**  Filename: database.c
**
**  Made by: Walkoss on 10/11/2017.
**
**  Description: 
*/

#include "database.h"

/**
 * Init database
 * @param databaseName
 * @return
 */
Database *initDatabase(const char *databaseName) {
    Database *database;
    char *databaseNameCopy;

    database = xmalloc(sizeof(Table), __func__);
    databaseNameCopy = xmalloc(sizeof(strlen(databaseName)) + 1, __func__);

    if (database && databaseNameCopy) {
        database->isUsed = 0;
        database->name = strcpy(databaseNameCopy, databaseName);
        database->tableHead = NULL;
    }

    return database;
}

/**
 * Free Database
 * @param database
 */
void freeDatabase(Database *database) {
    free(database->name);
    database->isUsed = 0;
    // TODO: freeTables
    free(database);
}

/**
 * Use Database
 * @param database
 * @return
 */
Database *useDatabase(Database* database) {
    database->isUsed = 1;
    initTables(database);

    return database;
}

/**
 * Create a database
 * @param databaseName
 * @return 0 if success, 1 for error
 */
int createDatabase(Database *database) {
    char *path;

    path = getDatabasePath(database->name);
    if (!path)
        return 1;
    if (mkdir(path, 0777) == -1) {
        fprintf(stderr, "An error has occured when creating database '%s': "
                "%s\n", database->name, strerror(errno));
        free(path);
        return 1;
    }

    free(path);
    return 0;
}

/**
 * Remove file 'fpath'
 * Function pointer passed as parameter to ntfw function, see removeDatabase function below
 * @param fpath
 * @param sb
 * @param tflag
 * @param ftwbuf
 * @return
 */
int removeFile(const char *fpath,
               const struct stat *sb,
               int tflag,
               struct FTW *ftwbuf) {
    if (remove(fpath) == -1) {
        fprintf(stderr,
                "An error has occured when removing directory/file '%s': "
                        "%s\n",
                fpath,
                strerror(errno));
        return 1;
    }
    return 0;
}

/**
 * Remove directory 'databaseName' and its content
 * @param databaseName
 * @return
 */
int dropDatabase(Database *database) {
    char *path;

    path = getDatabasePath(database->name);
    if (!path)
        return 1;

    if (nftw(path, removeFile, NOPENFD, FTW_DEPTH) == -1) {
        fprintf(stderr, "An error has occured when removing database '%s': "
                "%s\n", database->name, strerror(errno));
        free(path);
        return 1;
    }

    free(path);
    return 0;
}

/**
 * Based on RESOURCES_DIR, this function returns an absolute path to resources
 * Example: 'test' -> 'resources/test'
 * @param databaseName
 * @return path is success, NULL otherwise
 */
char *getDatabasePath(const char *databaseName) {
    char *path;

    path = xmalloc(strlen(RESOURCES_DIR) + strlen(databaseName) + 1, __func__);
    if (!path)
        return NULL;
    strcpy(path, RESOURCES_DIR);
    strcat(path, databaseName);

    return path;
}