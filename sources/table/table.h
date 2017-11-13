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

int addTable(const char *databaseName, const char *tableName);
int removeTable(const char *databaseName, const char *tableName);

#endif //YAMLDB_TABLE_H
