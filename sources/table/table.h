/*
**  Filename: table.h
**
**  Made by: Alexis PETRILLO on 12/11/2017.
**
**  Description: Header file for table, for mangage tables
*/

#ifndef YAMLDB_TABLE_H
#define YAMLDB_TABLE_H

typedef struct STable {
    char *name;
    unsigned int pk;
    struct SField *fieldHead;
    struct STable *next;
} Table;

#endif //YAMLDB_TABLE_H
