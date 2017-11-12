/*
  Filename: table.h

  Made by: Alexis PETRILLO on 12/11/2017.

**  Description: 
*/
#ifndef YAMLDB_TABLE_H
#define YAMLDB_TABLE_H

#define RESOURCES_DIR "resources/"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "../utils/xmalloc.h"

int removeTable(const char *databaseName, const char *tableName);

#endif //YAMLDB_TABLE_H
