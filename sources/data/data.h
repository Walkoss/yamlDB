/*
**  Filename: data.h
**
**  Made by: Alexis PETRILLO on 25/11/2017.
**
**  Description: 
*/
#ifndef YAMLDB_DATA_H
#define YAMLDB_DATA_H

typedef struct SData {
    char *value;
    struct SField *field;
    struct SData *next;
} Data;

typedef struct SCondition {
    char *key;
    char *value;
} Condition;

Data *dataListLast(Data *node);

void dataListAppend(Data **node, Data *newNode);

#endif //YAMLDB_DATA_H
