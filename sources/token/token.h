/*
**  Filename: token.h
**
**  Made by: Walkoss on 18/11/2017.
**
**  Description: 
*/

#ifndef YAMLDB_TOKEN_H
#define YAMLDB_TOKEN_H

#include <string.h>

#define NUM_OP_KW_TOKENS 14
#define MAX_LEXICAL_VALUE 256

typedef enum {
    T_ILLEGAL = -1,
    T_EOS = 0, // \0
    T_LIT_IDENTIFIER, // is_admin
    T_LIT_INT, // 44
    T_LIT_FLOAT, // 3.44
    T_LIT_STR_SIMPLE_QUOTE, // 'test'
    T_LIT_STR_DOUBLE_QUOTE, // "test"
    T_LIT_STR_BACK_QUOTE, // `test`
    T_OP_LPAREN, // (
    T_OP_RPAREN, // )
    T_OP_SEMICOLON, // ;
    T_OP_COMMA, // ,
    T_OP_EQUAL, // =

    //  Database keywords
    T_KW_CREATE,
    T_KW_DROP,
    T_KW_USE,
    T_KW_INSERT,
    T_KW_INTO,
    T_KW_VALUES,
    T_KW_UPDATE,
    T_KW_FROM,
    T_KW_SET,
    T_KW_WHERE,
    T_KW_DELETE,
    T_KW_DATABASE,
    T_KW_TABLE,
    T_KW_INT,
    T_KW_FLOAT,
    T_KW_VARCHAR,
    T_KW_CHAR
} TokenType;

typedef struct {
    TokenType token;
    char value[MAX_LEXICAL_VALUE];
} TokenHash;

char *tokenTypeAsString(TokenType tokenType);

#endif //YAMLDB_TOKEN_H
