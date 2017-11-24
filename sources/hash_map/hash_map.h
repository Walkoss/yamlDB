/*
**  Filename: hash_map.h
**
**  Made by: Walkoss on 19/11/2017.
**
**  Description: 
*/

#ifndef YAMLDB_HASH_MAP_H
#define YAMLDB_HASH_MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "../utils/xmalloc.h"

typedef struct SHashMapElem {
    char *key;
    void *value;
    struct SHashMapElem *next;
} HashMapNode;

typedef struct {
    size_t size;
    HashMapNode **table;
} HashMap;

HashMap *hashMapInit(size_t size);

void hashMapFree(HashMap *hashMap);

void hashMapPut(HashMap *hashMap, char *key, void *value);

void *hashMapGet(HashMap *hashMap, char *key);

void hashMapRemove(HashMap *hashMap, char *key);

#endif //YAMLDB_HASH_MAP_H
