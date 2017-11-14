/*
**  Filename: database.h
**
**  Made by: Walkoss on 10/11/2017.
**
**  Description: 
*/

#ifndef YAMLDB_DATABASE_H
#define YAMLDB_DATABASE_H

#define RESOURCES_DIR "resources/"
#define NOPENFD 20

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ftw.h>
#include "../utils/xmalloc.h"

int addDatabase(const char *databaseName);

int removeDatabase(const char *databaseName);

char *getDatabasePath(const char *databaseName);

#endif //YAMLDB_DATABASE_H
