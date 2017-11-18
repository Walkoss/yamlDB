/*
**  Filename: token.h
**
**  Made by: Walkoss on 18/11/2017.
**
**  Description: 
*/

#ifndef YAMLDB_TOKEN_H
#define YAMLDB_TOKEN_H

#include <stdio.h>


typedef enum {
    ILLEGAL = -1,
    EOS, // \0
    NEWLINE, // \n
    IDENTIFIER, // is_admin
    INT, // 44
    FLOAT, // 3.44
    STRING_SIMPLE_QUOTE, // 'test'
    STRING_DOUBLE_QUOTE, // "test"
    STRING_BACK_QUOTE, // `test`
    LPAREN, // (
    RPAREN, // )
    SEMICOLON, // ;
    COMMA, // ,
    OP_ASSIGN, // =
    OP_EQ, // ==

    // Database keywords
    CREATE,
    DROP,
    USE,
    DATABASE,
    TABLE
} TokenType;

typedef struct {
    int len;
    TokenType type;
    struct {
        float asFloat;
        int asInt;
        char *asString;
    } value;
} Token;

char *tokenTypeAsString(TokenType);

void tokenInspect(Token);

#endif //YAMLDB_TOKEN_H
