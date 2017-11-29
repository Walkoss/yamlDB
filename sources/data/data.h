/*
**  Filename: data.h
**
**  Made by: Alexis PETRILLO on 25/11/2017.
**
**  Description: 
*/
#ifndef YAMLDB_DATA_H
#define YAMLDB_DATA_H

typedef struct SDATA {
    char *value;
    struct SField *field;
    struct SDATA *next;
} Data;

typedef struct SCONDITION {
    char *key;
    char *value;
} Condition;

#endif //YAMLDB_DATA_H
