/*
**  Filename: table.h
**
**  Made by: Alexis PETRILLO on 12/11/2017.
**
**  Description: Header file for table, for mangage tables
*/
#ifndef YAMLDB_TABLE_H
#define YAMLDB_TABLE_H

#include "../utils/xmalloc.h"
#include "../field/field.h"

typedef struct STable {
    char *name;
    // TODO: init pk to 0
    unsigned int pk;
    Field *fieldHead;
    struct STable *next;
} Table;


Table *initTables(char *tableName);

void freeTables(Table *table);

//int addFields(Table *table, FILE *file);

int createTable(const char *databaseName, Table *table);

int removeTable(const char *databaseName, Table *table);

#endif //YAMLDB_TABLE_H
