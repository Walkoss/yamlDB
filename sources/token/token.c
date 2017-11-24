/*
**  Filename: token.c
**
**  Made by: Walkoss on 18/11/2017.
**
**  Description: 
*/

#include "token.h"

// TODO: Try to not set global variable
TokenHash tokens[] = {
        {.token = T_ILLEGAL, .value = "illegal"},
        {.token = T_EOS, .value = "end-of-source"},
        {.token = T_LIT_IDENTIFIER, .value = "identifier"},
        {.token = T_LIT_INT, .value = "int"},
        {.token = T_LIT_FLOAT, .value = "float"},
        {.token = T_LIT_STR_SIMPLE_QUOTE, .value = "simple-quote"},
        {.token = T_LIT_STR_DOUBLE_QUOTE, .value = "double-quote"},
        {.token = T_LIT_STR_BACK_QUOTE, .value = "back-quote"},
        {.token = T_OP_LPAREN, .value = "left-parenthesis"},
        {.token = T_OP_RPAREN, .value = "right-parenthesis"},
        {.token = T_OP_SEMICOLON, .value = "semicolon"},
        {.token = T_OP_COMMA, .value = "comma"},
        {.token = T_OP_ASSIGN, .value = "operator-assign"},
        {.token = T_OP_EQUALS, .value = "operator-equal"},
        {.token = T_KW_CREATE, .value = "create"},
        {.token = T_KW_DROP, .value = "drop"},
        {.token = T_KW_USE, .value = "use"},
        {.token = T_KW_DATABASE, .value = "database"},
        {.token = T_KW_TABLE, .value = "table"},
        {.value = "NULL"}
};

char *tokenTypeAsString(TokenType type) {
    int i;

    i = 0;
    while (strcmp(tokens[i].value, "NULL") != 0) {
        if (tokens[i].token == type) {
            return tokens[i].value;
        }
        i++;
    }
    return "illegal";
}