/*
**  Filename: interface.c
**
**  Made by: Alexis PETRILLO on 17/12/2017.
**
**  Description: 
*/

#include "../database/database.h"

#define SQL_QUERY_SIZE_BUFFER 1024

/**
 * Read user input
 * @return char* if success, NULL for error
 */
char *getUserInput() {
    char *buffer;
    size_t len;

    buffer = xmalloc((SQL_QUERY_SIZE_BUFFER + 1), __func__);
    if (!buffer)
        return NULL;

    if (fgets(buffer, SQL_QUERY_SIZE_BUFFER, stdin) == NULL) {
        free(buffer);
        return NULL;
    }
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    return buffer;
}