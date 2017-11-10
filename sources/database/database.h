/*
**  Filename: database.h
**
**  Made by: Walkoss on 10/11/2017.
**
**  Description: 
*/

#ifndef YAMLDB_DATABASE_H
#define YAMLDB_DATABASE_H

#define RESOURCES_DIR "resources/"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "../utils/xmalloc.h"

int addDatabase(const char *databaseName);

#endif //YAMLDB_DATABASE_H
