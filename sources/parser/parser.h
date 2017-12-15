/*
**  Filename: parser.h
**
**  Made by: Walkoss on 22/11/2017.
**
**  Description: 
*/

#include "../lexer/lexer.h"
#include "../statement/statement.h"

#ifndef YAMLDB_PARSER_H
#define YAMLDB_PARSER_H

typedef struct {
    Lexer *lexer;
    char* error;
    int hasError;
} Parser;

typedef struct SStmtFunction
{
    int	(*functionPtr)(Parser *);
}  StmtFunction;

Parser *parserInit(char *buffer);

void parserFree(Parser *parser);

void parse(Parser *parser);

int is(Parser *parser, TokenType tokenType);

int accept(Parser *parser, TokenType tokenType);

#endif //YAMLDB_PARSER_H
