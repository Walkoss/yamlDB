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
    Field *fieldNext;
} Table;


//
//Table *initialization();
//
//int newField(Table *table, char *name, char *type);
//
//int addFields(Table *table, FILE *file);

int addTable(const char *databaseName, const char *tableName, Table *table);

int removeTable(const char *databaseName, const char *tableName);

#endif //YAMLDB_TABLE_H
