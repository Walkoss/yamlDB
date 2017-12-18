/*
**  Filename: interface.h
**
**  Made by: Iliasse WAHBI on 14/11/2017.
**
**  Description:
*/
#ifndef YAMLDB_INTERFACE_H
#define YAMLDB_INTERFACE_H

#define RESOURCES_DIR "resources/"

#include <ctype.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "../utils/xmalloc.h"
#include "../database/database.h"

typedef struct SSentences
{
    char *sentence;
} TSentences;

FieldType getEnum(long type);

#endif //YAMLDB_INTERFACE_H