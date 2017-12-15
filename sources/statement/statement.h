/*
**  Filename: statement.h
**
**  Made by: Walkoss on 10/12/2017.
**
**  Description:
*/

#ifndef YAMLDB_STATEMENT_H
#define YAMLDB_STATEMENT_H

#include "../parser/parser.h"

// CREATE
int stmtCreate(Parser *parser, Database **database);

int stmtCreateDatabase(Parser *parser, Database **database);

int stmtCreateTable(Parser *parser, Database **database);

// USE
int stmtUseDatabase(Parser *parser, Database **database);

// DROP
int stmtDrop(Parser *parser, Database **database);

int stmtDropDatabase(Parser *parser, Database **database);

int stmtDropTable(Parser *parser, Database **database);

// INSERT
int stmtInsert(Parser *parser, Database **database);

// UPDATE
int stmtUpdate(Parser *parser, Database **database);

// DELETE
int stmtDelete(Parser *parser, Database **database);

#endif //YAMLDB_STATEMENT_H
