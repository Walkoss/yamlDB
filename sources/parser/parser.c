/*
**  Filename: parser.c
**
**  Made by: Walkoss on 22/11/2017.
**
**  Description: 
*/

#include "parser.h"

const StmtFunction stmtFunctions[] = {
        {stmtCreate},
        {stmtUseDatabase},
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
    fprintf(stderr, "%s\n", parser->error);
}

void parse(Parser *parser, Database **database) {
    int i;
    int returnValue;
    int stop;

    stop = 0;
    getToken(parser->lexer);
    while (!lexerIsEos(parser->lexer) && stop == 0) {
        i = 0;
        while (stmtFunctions[i].functionPtr != NULL) {
            returnValue = stmtFunctions[i].functionPtr(parser, database);
            if (returnValue != 0 && returnValue != -1) {
                if (parser->lexer->tok == T_ILLEGAL) {
                    lexerDisplayError(parser->lexer);
                } else if (parser->hasError) {
                    parserDisplayError(parser);
                }
                stop = 1;
                break;
            }
            i++;
        }
    }

    /*if ((*database)->selectedData)
        printf("%s: %s\n", (*database)->selectedData->key, (*database)->selectedData->value);
    while ((*database)->selectedData != NULL) {
        if ((*database)->selectedData->key)
            printf("%s: %s\n", (*database)->selectedData->key, (*database)->selectedData->value);
        else
            printf("-\n");
        (*database)->selectedData = (*database)->selectedData->next;
    }*/
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