/*
**  Filename: token.c
**
**  Made by: Walkoss on 18/11/2017.
**
**  Description: 
*/

#include "token.h"

char *tokenTypeAsString(TokenType type) {
    Token tokens[] = {
            {.type = ILLEGAL, .value.asString = "illegal"},
            {.type = EOS, .value.asString = "end-of-source"},
            {.type = NEWLINE, .value.asString = "newline"},
            {.type = IDENTIFIER, .value.asString = "identifier"},
            {.type = INT, .value.asString = "int"},
            {.type = FLOAT, .value.asString = "float"},
            {.type = STRING_SIMPLE_QUOTE, .value.asString = "simple-quote"},
            {.type = STRING_DOUBLE_QUOTE, .value.asString = "double-quote"},
            {.type = STRING_BACK_QUOTE, .value.asString = "back-quote"},
            {.type = LPAREN, .value.asString = "left-parenthesis"},
            {.type = RPAREN, .value.asString = "right-parenthesis"},
            {.type = SEMICOLON, .value.asString = "semicolon"},
            {.type = COMMA, .value.asString = "comma"},
            {.type = OP_ASSIGN, .value.asString = "operator-assign"},
            {.type = OP_EQ, .value.asString = "operator-equal"},
            {.type = CREATE, .value.asString = "create"},
            {.type = DROP, .value.asString = "drop"},
            {.type = USE, .value.asString = "use"},
            {.type = DATABASE, .value.asString = "database"},
            {.type = TABLE, .value.asString = "table"},
            {.value.asString = NULL},
    };
    int i;

    i = 0;
    while (tokens[i].value.asString != NULL) {
        if (tokens[i].type == type) {
            return tokens[i].value.asString;
        }
        i++;
    }
    return "unknown type";
}

void tokenInspect(Token token) {
    switch (token.type) {
        case STRING_SIMPLE_QUOTE:
            printf("string: '%s'\n", token.value.asString);
            break;
        case STRING_DOUBLE_QUOTE:
            printf("string: \"%s\"\n", token.value.asString);
            break;
        case STRING_BACK_QUOTE:
            printf("string: `%s`\n", token.value.asString);
            break;
        case IDENTIFIER:
            printf("identifier: %s\n", token.value.asString);
            break;
        case INT:
            printf("int: %d\n", token.value.asInt);
            break;
        default:
            printf("%s\n", tokenTypeAsString(token.type));
    }
}