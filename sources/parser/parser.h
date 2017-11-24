/*
**  Filename: parser.h
**
**  Made by: Walkoss on 22/11/2017.
**
**  Description: 
*/

#ifndef YAMLDB_PARSER_H
#define YAMLDB_PARSER_H

#include "../lexer/lexer.h"

typedef struct {
    Lexer *lexer;
} Parser;

Parser *parserInit();

void parserFree(Parser *parser);

void parse(Parser *parser);

#endif //YAMLDB_PARSER_H
