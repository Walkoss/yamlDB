/*
**  Filename: interface.h
**
**  Made by: Iliasse WAHBI on 14/11/2017.
**
**  Description:
*/
#ifndef YAMLDB_INTERFACE_H
#define YAMLDB_INTERFACE_H

#define RESOURCES_DIR "resources/"

#include <ctype.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "../utils/xmalloc.h"
#include "../database/database.h"
#include "../table/table.h"

typedef struct SDatabaseFunc {
    int (*function)(Database*);
} TDatabaseFunc;

typedef struct STableFunc {
    int (*function)(Database*, Table*);
} TTableFunc;

typedef struct SSentences
{
    char *sentence;
} TSentences;

int userInterface();
Database *choice(long userChoice, Database *database);
void printInstruction(int sentence);
char *getUserInput(int size);
int isAllDigit(const char* str);
void isSuccess(int result);
Database *createField(Database *database, Table* table, char *name);
int preCreateTable(Database* database, Table* table);
FieldType getEnum();
char *getFieldName(Table *table);
int getDatabaseFuncLength();
int getTableFuncLength();

#endif //YAMLDB_INTERFACE_H