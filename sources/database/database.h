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
#include <sys/types.h>
#include <dirent.h>
#include "../utils/xmalloc.h"

typedef enum EFieldType {
    PK = 1,
    INT,
    FLOAT,
    CHAR,
    VARCHAR,
} FieldType;

typedef struct SField {
    char *name;
    FieldType type;
    struct SField *next;
} Field;

typedef struct STable {
    char *name;
    unsigned int pk;
    struct SField *fieldHead;
    struct STable *next;
} Table;

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

void initTables(Database *database);
int createTable(Database *database, Table *table);
Table *findTable(Database *database, char *tableName);
int dropTable(Database *database, Table *table);
int freeTable(Database *database, Table *table);
int freeTables(Database *database);
char *getTablePath(const char *databaseName, const char *tableName);

int initFields(Database *database, Table *table);
int addFieldsInFile(Database *database, Table *table, FILE *file);
int freeFields(Table *table);

#endif //YAMLDB_DATABASE_H
