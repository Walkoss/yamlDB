/*
**  Filename: xmalloc.c
**
**  Made by: Walkoss on 10/11/2017.
**
**  Description: 
*/

#include "xmalloc.h"

void *xmalloc(size_t size, const char *functionName) {
    void *ptr;

    ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Malloc failed for %s function", functionName);
    }
    return ptr;
}