/*
**  Filename: statement.c
**
**  Made by: Walkoss on 10/12/2017.
**
**  Description:
*/

#include "statement.h"

/**
 * Create Database Statement ("CREATE DATABASE databaseName;")
 * @param parser
 */
int stmtCreateDatabase(Parser *parser) {
    char *databaseName;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "database name is missing after CREATE DATABASE\n");
        return 0;
    }

    databaseName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting after %s\n", databaseName);
        return 0;
    }

    printf("CREATE DATABASE %s\n", databaseName);

    return 1;
}

/**
 * Create Table Statement ("CREATE TABLE tableName(field INT, ..);")
 * @param parser
 */
int stmtCreateTable(Parser *parser) {
    char *tableName;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after CREATE TABLE\n");
        return 0;
    }

    printf("table name = %s p = %p\n", parser->lexer->value, &parser->lexer->value);
    tableName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);

    if (!accept(parser, T_OP_LPAREN)) {
        parser->hasError = 1;
        sprintf(parser->error, "left parenthesis missing after %s\n", tableName);
        return 0;
    }

    do {
        if (!is(parser, T_LIT_IDENTIFIER)) {
            parser->hasError = 1;
            sprintf(parser->error, "field name is missing\n");
            return 0;
        }

        printf("Field name = %s p = %p\n", parser->lexer->value, &parser->lexer->value);
        accept(parser, T_LIT_IDENTIFIER);

        if (!is(parser, T_KW_INT) && !is(parser, T_KW_FLOAT) && !is(parser, T_KW_VARCHAR)) {
            parser->hasError = 1;
            sprintf(parser->error, "field type is missing (INT|FLOAT|VARCHAR)\n");
            return 0;
        }

        printf("Field type = %s p = %p\n", tokenTypeAsString(parser->lexer->tok), &parser->lexer->value);
        accept(parser, parser->lexer->tok);
    } while (accept(parser, T_OP_COMMA));

    if (!accept(parser, T_OP_RPAREN)) {
        parser->hasError = 1;
        sprintf(parser->error, "right parenthesis missing\n");
        return 0;
    }

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting )\n");
        return 0;
    }

    printf("CREATE TABLE %s\n", tableName);

    return 1;
}


/**
 * Create [Database|Table] Statement
 * @param parser
 */
int stmtCreate(Parser *parser) {
    if (!accept(parser, T_KW_CREATE)) {
        return 0;
    }

    if (accept(parser, T_KW_DATABASE)) {
        return stmtCreateDatabase(parser);
    } else if (accept(parser, T_KW_TABLE)) {
        return stmtCreateTable(parser);
    }

    parser->hasError = 1;
    sprintf(parser->error, "Missing keyword after CREATE (DATABASE|TABLE)");
    return 0;
}


/**
 * Drop [Database|Table] Statement
 * @param parser
 */
int stmtDrop(Parser *parser) {
    if (!accept(parser, T_KW_DROP)) {
        return 0;
    }

    if (accept(parser, T_KW_DATABASE)) {
        return stmtDropDatabase(parser);
    } else if (accept(parser, T_KW_TABLE)) {
        return stmtDropTable(parser);
    }

    parser->hasError = 1;
    sprintf(parser->error, "Missing keyword after DROP (DATABASE|TABLE)");
    return 0;
}

/**
 * Use Database Statement ("USE databaseName;")
 * @param parser
 */
int stmtUseDatabase(Parser *parser) {
    char *databaseName;

    if (!accept(parser, T_KW_USE)) {
        return 0;
    }

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "database name is missing after USE keyword\n");
        return 0;
    }

    databaseName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting after %s\n", databaseName);
        return 0;
    }

    printf("USE DATABASE %s\n", databaseName);


    return 1;
}

/**
 * DROP DATABASE Statement ("DROP DATABASE databaseName;")
 * @param parser
 */
int stmtDropDatabase(Parser *parser) {
    char *databaseName;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "database name is missing after DROP DATABASE\n");
        return 0;
    }

    databaseName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting after %s\n", databaseName);
        return 0;
    }

    printf("DROP DATABASE %s\n", databaseName);

    return 1;
}

/**
 * DROP TABLE Statement ("DROP TABLE tableName;")
 * @param parser
 */
int stmtDropTable(Parser *parser) {
    char *tableName;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after DROP TABLE\n");
        return 0;
    }

    tableName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting after %s\n", tableName);
        return 0;
    }

    printf("DROP TABLE %s\n", tableName);

    return 1;
}