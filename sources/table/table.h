/*
**  Filename: table.h
**
**  Made by: Alexis PETRILLO on 12/11/2017.
**
**  Description: Header file for table, for mangage tables
*/
#include "../database/database.h"

#ifndef YAMLDB_TABLE_H
#define YAMLDB_TABLE_H

#include "../field/field.h"

typedef struct STable {
    char *name;
    // TODO: init pk to 0
    unsigned int pk;
    Field *fieldHead;
    struct STable *next;
} Table;


Table *initTables(Database *database);
/*
void freeTables(Database *database);

//int addFields(Table *table, FILE *file);

int createTable(Database *database, Table *table);

int removeTable(Database *database, Table *table);
*/
char *getTablePath(const char *databaseName, const char *tableName);

#endif //YAMLDB_TABLE_H
