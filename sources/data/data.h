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
    void *value;
    struct SField *field;
    struct SData *next;
} Data;

#endif //YAMLDB_DATA_H
