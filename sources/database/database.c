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

    path = getDatabasePath(databaseName);
    if (!path)
        return 1;
    if (mkdir(path, 0777) == -1) {
        fprintf(stderr, "An error has occured when creating database '%s': "
                "%s\n", databaseName, strerror(errno));
        free(path);
        return 1;
    }

    free(path);
    return 0;
}

int removeFile(const char *fpath,
               const struct stat *sb,
               int tflag,
               struct FTW *ftwbuf) {
    if (tflag == FTW_DP) {
        if (rmdir(fpath) == -1) {
            fprintf(stderr,
                    "An error has occured when removing directory '%s': %s\n",
                    fpath,
                    strerror(errno));
            return 1;
        }
    } else if (tflag == FTW_F) {
        if (unlink(fpath) == -1) {
            fprintf(stderr,
                    "An error has occured when removing file '%s': %s\n",
                    fpath,
                    strerror(errno));
            return 1;
        }
    }
    return 0;
}

int removeDatabase(const char *databaseName) {
    char *path;

    path = getDatabasePath(databaseName);
    if (!path)
        return 1;

    if (nftw(path, removeFile, NOPENFD, FTW_DEPTH) == -1) {
        fprintf(stderr, "An error has occured when removing database '%s': "
                "%s\n", databaseName, strerror(errno));
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