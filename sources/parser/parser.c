/*
**  Filename: parser.c
**
**  Made by: Walkoss on 22/11/2017.
**
**  Description: 
*/

#include "parser.h"
//
//Parser *parserInit() {
//    Parser *parser;
//
//    parser = xmalloc(sizeof(Parser), __func__);
//    if (parser != NULL) {
//        parser->lexer = lexerInit();
//    }
//    return parser;
//}
//
//void parserFree(Parser *parser) {
//    if (parser != NULL) {
//        lexerFree(parser->lexer);
//        free(parser);
//    }
//}
//
//void parse(Parser *parser) {
//    while (parser->lexer->tok != T_EOF) {
//        getToken(parser->lexer);
//    }
//}