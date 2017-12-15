/*
**  Filename: lexer.h
**
**  Made by: Walkoss on 18/11/2017.
**
**  Description: 
*/

#include "../token/token.h"
#include "../hash_map/hash_map.h"
#include "../utils/xmalloc.h"
#include "../utils/to_lower.h"

#ifndef YAMLDB_LEXER_H
#define YAMLDB_LEXER_H

#include <stdlib.h>
#include <ctype.h>

typedef struct {
    char *error;                    // string containing an error if tok == T_ILLEGAL
    int cur;                        // current character in buffer
    size_t lineNb;                  // line number in buffer
    size_t curPos;                  // current character position in line
    TokenType tok;                  // current TokenType (INT, FLOAT, etc..)
    char *value;                    // value parsed in buffer (INT, STR, FLOAT)
    char *buffer;                   // string to parse
    size_t cursor;                  // cursor in buffer (index)
    size_t bufferSize;              // size of buffer
    HashMap *tokensHashTable;       // hash map containing all the operators
    // and keywords token
} Lexer;

Lexer *lexerInit(char *buffer);

void lexerFree(Lexer *lexer);

TokenType getToken(Lexer *lexer);

void tokenInspect(Lexer *lexer);

void lexerDisplayError(Lexer *lexer);

int lexerIsEos(Lexer *lexer);

#endif //YAMLDB_LEXER_H
