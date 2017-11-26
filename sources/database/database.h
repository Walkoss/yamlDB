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
#define BUFFER_SIZE 1024

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ftw.h>
#include <sys/types.h>
#include <dirent.h>
#include "../utils/xmalloc.h"
#include "../table/table.h"
#include "../field/field.h"
#include "../data/data.h"

typedef struct SDatabase {
    char *name;
    int isUsed;
    struct STable *tableHead;
} Database;

Database *initDatabase(const char *databaseName);
int createDatabase(Database *database);
Database *useDatabase(Database *database);
int dropDatabase(Database *database);
int freeDatabase(Database *database);
char *getDatabasePath(const char *databaseName);
int debugDatabase(Database *database);

int initTables(Database *database);
int createTable(Database *database, Table *table);
Table *findTable(Database *database, char *tableName);
int dropTable(Database *database, Table *table);
int freeTable(Database *database, Table *table);
int freeTables(Database *database);
char *getTablePath(const char *databaseName, const char *tableName);

int initFields(Database *database, Table *table);
int addFieldsInFile(Database *database, Table *table);
int freeFields(Table *table);

int addData(Database *database, Table *table, Data *data);
int removeData(Database *database, Table *table, Condition *condition);

#endif //YAMLDB_DATABASE_H
