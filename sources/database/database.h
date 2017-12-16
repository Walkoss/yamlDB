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
#include "../utils/strsplit.h"
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
int initFieldsInStruct(FILE *file, Table *table, Field *field);
int addFieldsInFile(Database *database, Table *table);
int freeFields(Table *table);

int addData(Database *database, Table *table, Data *data);
int openFilesForRemoving(Database *database, Table *table, Condition *condition);
int openFilesForUpdating(Database *database, Table *table, Data *data, Condition *condition);
void removeData(FILE *file, FILE *filetmp, Condition *condition);
void updateData(FILE *file, FILE *filetmp, Data *data, Condition *condition);
long updateDataOnFile(FILE *file, FILE *fileTmp, Data *data);

int selectData(Database *database, Table *table, Field *field, Condition *condition);
void selectMethod(FILE *file, Field *field, Condition *condition);
long displayAllData(FILE *file);
int displayAllDataWithoutCondition(FILE *file);
long BrowseSingleData(FILE *file, Field *field);
long displaySingleData(FILE *file, Field *currentField);
long isConditionFulfilled(FILE *file, Condition *condition);

int openFilesForInnerJoin(Database *database, Table *table1, Table *table2, char *key, char *key2, void *field,
                          void *condition);
void selectFuncInnerJoin(FILE *file, FILE *file2, char* key, char* key2, Field *field, Condition *condition);
int innerJoin(Table *table1, Table *table2, char* key, char* key2, Field *field, Condition *condition);

#endif //YAMLDB_DATABASE_H
