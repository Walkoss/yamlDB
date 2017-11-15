/*
**  Filename: field.h
**
**  Made by: Walkoss on 15/11/2017.
**
**  Description: 
*/

#ifndef YAMLDB_FIELD_H
#define YAMLDB_FIELD_H

typedef enum EFieldType {
    PK = 1,
    INT,
    FLOAT,
    CHAR,
    VARCHAR,
} FieldType;

typedef struct SField {
    char *name;
    FieldType type;
} Field;

#endif //YAMLDB_FIELD_H
