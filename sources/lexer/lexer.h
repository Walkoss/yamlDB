/*
**  Filename: lexer.h
**
**  Made by: Walkoss on 18/11/2017.
**
**  Description: 
*/

#ifndef YAMLDB_LEXER_H
#define YAMLDB_LEXER_H

#include <stdio.h>
#include <memory.h>
#include <ctype.h>
#include "../token/token.h"
#include "../utils/xmalloc.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

typedef struct {
    char *error;
    int stash;
    int linenumber;
    FILE *stream;
    Token token;
} Lexer;

void lexerInit(Lexer *lexer, FILE *stream);

int lexerNext(Lexer *lexer);

#endif //YAMLDB_LEXER_H
