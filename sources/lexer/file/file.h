/*
**  Filename: file.h
**
**  Made by: Walkoss on 19/11/2017.
**
**  Description: 
*/

#ifndef YAMLDB_FILE_H
#define YAMLDB_FILE_H

#include <stdlib.h>
#include "../../utils/xmalloc.h"

#define MAX_READ_SIZE 1024

typedef struct {
    size_t cursor;
    size_t size;
    char *buf;
} File;

File *fileInit();

void fileFree(File *file);

void fileRead(File *file);

int fileNextChar(File *file);

#endif //YAMLDB_FILE_H
