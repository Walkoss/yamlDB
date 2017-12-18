/*
**  Filename: parser.c
**
**  Made by: Walkoss on 22/11/2017.
**
**  Description: 
*/

#include "parser.h"
#include "../lexer/lexer.h"
#include "../print_color/print_color.h"

const StmtFunction stmtFunctions[] = {
        {stmtUseDatabase},
        {stmtCreate},
        {stmtDrop},
        {stmtInsert},
        {stmtUpdate},
        {stmtDelete},
        {stmtSelect},
        {NULL}
};

Parser *parserInit(char *buffer) {
    Parser *parser;

    parser = xmalloc(sizeof(Parser), __func__);
    if (parser != NULL) {
        parser->lexer = lexerInit(buffer);
        parser->hasError = 0;
        parser->error = xmalloc(MAX_LEXICAL_VALUE, __func__);
    }
    return parser;
}

void parserFree(Parser *parser) {
    if (parser != NULL) {
        lexerFree(parser->lexer);
        if (parser->error) {
            free(parser->error);
        }
        free(parser);
    }
}

void parserDisplayError(Parser *parser) {
    fprintf(stderr, "%s%s at line %zd and position %zd%s",
            COLOR_RED, parser->error, parser->lexer->lineNb, parser->lexer->curPos, COLOR_RESET);
}

int parse(Parser *parser, Database **database) {
    int i;
    int returnValue;

    getToken(parser->lexer);
    while (parser->lexer->tok != T_EOS) {
        i = 0;
        returnValue = -1;
        while (stmtFunctions[i].functionPtr != NULL) {
            returnValue = stmtFunctions[i].functionPtr(parser, database);
            if (returnValue != 0 && returnValue != -1) {
                if (parser->lexer->tok == T_ILLEGAL) {
                    lexerDisplayError(parser->lexer);
                } else if (parser->hasError) {
                    parserDisplayError(parser);
                }
                return returnValue;
            } else if (returnValue == 0) {
                break;
            }
            i++;
        }
        if (returnValue == -1) {
            fprintf(stderr, "%sSyntax Error: command not found%s", COLOR_RED, COLOR_RESET);
            return 1;
        }
    }
    return 0;
}

int is(Parser *parser, TokenType tokenType) {
    if (parser->lexer->tok == tokenType) {
        return 1;
    }
    return 0;
}

int accept(Parser *parser, TokenType tokenType) {
    if (parser->lexer->tok == tokenType) {
        getToken(parser->lexer);
        return 1;
    }
    return 0;
}