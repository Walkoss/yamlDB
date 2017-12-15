/*
**  Filename: statement.h
**
**  Made by: Walkoss on 10/12/2017.
**
**  Description:
*/

#ifndef YAMLDB_STATEMENT_H
#define YAMLDB_STATEMENT_H

#include "../lexer/lexer.h"
#include "../parser/parser.h"

// CREATE
int stmtCreate(Parser *parser);

int stmtCreateDatabase(Parser *parser);

int stmtCreateTable(Parser *parser);

// USE
int stmtUseDatabase(Parser *parser);

// DROP
int stmtDrop(Parser *parser);

int stmtDropDatabase(Parser *parser);

int stmtDropTable(Parser *parser);

// INSERT
int stmtInsert(Parser *parser);

// UPDATE
int stmtUpdate(Parser *parser);

// DELETE
int stmtDelete(Parser *parser);

#endif //YAMLDB_STATEMENT_H
