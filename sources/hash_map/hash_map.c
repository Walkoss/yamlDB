/*
**  Filename: hash_map.c
**
**  Made by: Walkoss on 19/11/2017.
**
**  Description: 
*/

#include "hash_map.h"

HashMap *hashMapInit(size_t size) {
    HashMap *hashMap;
    size_t i;

    hashMap = xmalloc(sizeof(HashMap), __func__);
    if (hashMap == NULL)
        return NULL;

    hashMap->table = xmalloc(sizeof(HashMapNode *) * size, __func__);
    if (hashMap->table == NULL) {
        free(hashMap);
        return NULL;
    }

    for (i = 0; i < size; i++)
        hashMap->table[i] = NULL;

    hashMap->size = size;
    return hashMap;
}

void hashMapFree(HashMap *hashMap) {
    size_t i;

    if (hashMap == NULL)
        return;
    if (hashMap->table != NULL) {
        for (i = 0; i < hashMap->size; i++) {
            if (hashMap->table[i] != NULL) {
                free(hashMap->table[i]->key);
                free(hashMap->table[i]->value);
                free(hashMap->table[i]);
            }
        }
    }
    free(hashMap);
}

size_t hashMapHash(HashMap *hashMap, char *key) {
    size_t result;
    size_t i;

    i = 0;
    result = 0;
    while (result < ULONG_MAX && i < strlen(key)) {
        result = result << 8;
        result += key[i];
        i++;
    }

    return result % hashMap->size;
}

void hashMapPut(HashMap *hashMap, char *key, void *value) {
    size_t index;
    HashMapNode *prev;
    HashMapNode *cur;
    HashMapNode *node;

    index = hashMapHash(hashMap, key);
    prev = hashMap->table[index];
    cur = prev;

    // Check if key already exist
    while (cur != NULL) {
        if (!strcmp(key, cur->key)) {
            cur->value = value;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
    node = xmalloc(sizeof(HashMapNode), __func__);
    if (node != NULL) {
        node->key = strdup(key);
        node->value = value;
        node->next = NULL;
        if (cur == prev) {
            hashMap->table[index] = node;
        } else if (prev != NULL) {
            prev->next = node;
        }
    }
}

void *hashMapGet(HashMap *hashMap, char *key) {
    size_t index;
    HashMapNode *node;

    index = hashMapHash(hashMap, key);
    for (node = hashMap->table[index]; node != NULL; node = node->next) {
        if (node->key != NULL && !strcmp(key, node->key)) {
            return node->value;
        }
    }
    return NULL;
}

void hashMapRemove(HashMap *hashMap, char *key) {
    size_t index;
    HashMapNode *prev;
    HashMapNode *cur;

    index = hashMapHash(hashMap, key);
    prev = hashMap->table[index];
    cur = prev;

    // Check if key already exist
    while (cur != NULL) {
        if (!strcmp(key, cur->key)) {
            free(cur->key);
            if (prev != cur) {
                prev->next = (cur->next != NULL) ? cur->next : NULL;
            }
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}