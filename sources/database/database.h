/*
**  Filename: database.h
**
**  Made by: Walkoss on 10/11/2017.
**
**  Description: 
*/
#include "../table/table.h"
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

typedef struct SDatabase {
    char *name;
    int isUsed;
    Table *tableHead;
} Database;

Database *initDatabase(const char *databaseName);

Database *useDatabase(Database *);

int createDatabase(Database *);

int dropDatabase(Database *);

void freeDatabase(Database *);

char *getDatabasePath(const char *databaseName);

#endif //YAMLDB_DATABASE_H
