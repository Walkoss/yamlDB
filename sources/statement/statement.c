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
int stmtCreateDatabase(Parser *parser, Database **database) {
    char *databaseName;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "database name is missing after CREATE DATABASE\n");
        return 1;
    }

    databaseName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting after %s\n", databaseName);
        return 1;
    }

    *database = initDatabase(databaseName);
    return createDatabase(*database);
}

/**
 * Create Table Statement ("CREATE TABLE tableName(field INT, ..);")
 * @param parser
 */
int stmtCreateTable(Parser *parser, Database **database) {
    char *fieldName;
    FieldType fieldType;
    Field *field;
    Table *table;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after CREATE TABLE\n");
        return 1;
    }

    accept(parser, T_LIT_IDENTIFIER);
    table = xmalloc(sizeof(Table), __func__);
    table->name = strdup(parser->lexer->value);
    table->fieldHead = NULL;

    if (!accept(parser, T_OP_LPAREN)) {
        parser->hasError = 1;
        sprintf(parser->error, "left parenthesis missing after %s\n", table->name);
        return 1;
    }

    do {
        if (!is(parser, T_LIT_IDENTIFIER)) {
            parser->hasError = 1;
            sprintf(parser->error, "field name is missing\n");
            return 1;
        }

        fieldName = strdup(parser->lexer->value);
        accept(parser, T_LIT_IDENTIFIER);

        if (!is(parser, T_KW_INT) && !is(parser, T_KW_FLOAT) && !is(parser, T_KW_VARCHAR) && !is(parser, T_KW_CHAR)) {
            parser->hasError = 1;
            sprintf(parser->error, "field type is missing (INT|FLOAT|VARCHAR)\n");
            return 1;
        }

        switch (parser->lexer->tok) {
            case T_KW_INT:
                fieldType = INT;
                break;
            case T_LIT_FLOAT:
                fieldType = FLOAT;
                break;
            case T_KW_CHAR:
                fieldType = CHAR;
                break;
            case T_KW_VARCHAR:
                fieldType = VARCHAR;
                break;
            default:
                fieldType = VARCHAR;
        }

        field = xmalloc(sizeof(Field), __func__);
        if (field) {
            field->name = fieldName;
            field->type = fieldType;
            field->next = NULL;
        }

        fieldListAppend(&table->fieldHead, field);
        accept(parser, parser->lexer->tok);
    } while (accept(parser, T_OP_COMMA));

    if (!accept(parser, T_OP_RPAREN)) {
        parser->hasError = 1;
        sprintf(parser->error, "right parenthesis missing\n");
        return 1;
    }

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting )\n");
        return 1;
    }

    return createTable(*database, table);
}


/**
 * Create [Database|Table] Statement
 * @param parser
 */
int stmtCreate(Parser *parser, Database **database) {
    if (!accept(parser, T_KW_CREATE)) {
        return 1;
    }

    if (accept(parser, T_KW_DATABASE)) {
        return stmtCreateDatabase(parser, database);
    } else if (accept(parser, T_KW_TABLE)) {
        return stmtCreateTable(parser, database);
    }

    parser->hasError = 1;
    sprintf(parser->error, "Missing keyword after CREATE (DATABASE|TABLE)");
    return 1;
}


/**
 * Drop [Database|Table] Statement
 * @param parser
 */
int stmtDrop(Parser *parser, Database **database) {
    if (!accept(parser, T_KW_DROP)) {
        return 1;
    }

    if (accept(parser, T_KW_DATABASE)) {
        return stmtDropDatabase(parser, database);
    } else if (accept(parser, T_KW_TABLE)) {
        return stmtDropTable(parser, database);
    }

    parser->hasError = 1;
    sprintf(parser->error, "Missing keyword after DROP (DATABASE|TABLE)");
    return 1;
}

/**
 * Use Database Statement ("USE databaseName;")
 * @param parser
 */
int stmtUseDatabase(Parser *parser, Database **database) {
    char *databaseName;

    if (!accept(parser, T_KW_USE)) {
        return 1;
    }

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "database name is missing after USE keyword\n");
        return 1;
    }

    databaseName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting after %s\n", databaseName);
        return 1;
    }

    if (*database == NULL)
        *database = initDatabase(databaseName);
    return useDatabase(*database);
}

/**
 * DROP DATABASE Statement ("DROP DATABASE databaseName;")
 * @param parser
 */
int stmtDropDatabase(Parser *parser, Database **database) {
    char *databaseName;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "database name is missing after DROP DATABASE\n");
        return 1;
    }

    databaseName = strdup(parser->lexer->value);
    accept(parser, T_LIT_IDENTIFIER);

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting after %s\n", databaseName);
        return 1;
    }

    *database = initDatabase(databaseName);
    return dropDatabase(*database);
}

/**
 * DROP TABLE Statement ("DROP TABLE tableName;")
 * @param parser
 */
int stmtDropTable(Parser *parser, Database **database) {
    Table *table;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after DROP TABLE\n");
        return 1;
    }

    table = xmalloc(sizeof(Table), __func__);
    table->name = strdup(parser->lexer->value);
    accept(parser, T_LIT_IDENTIFIER);

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting after %s\n", table->name);
        return 1;
    }

    return dropTable(*database, table);
}

int stmtInsert(Parser *parser, Database **database) {
    char *tableName;
    int valuesCount;
    int columnsCount;

    valuesCount = 0;
    columnsCount = 0;

    if (!accept(parser, T_KW_INSERT)) {
        return 1;
    }

    if (!accept(parser, T_KW_INTO)) {
        parser->hasError = 1;
        sprintf(parser->error, "INTO keyword is missing after INSERT\n");
        return 1;
    }

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after INSERT INTO\n");
        return 1;
    }

    tableName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);

    // Columns definition (optional)
    if (accept(parser, T_OP_LPAREN)) {
        do {
            if (!is(parser, T_LIT_IDENTIFIER)) {
                parser->hasError = 1;
                sprintf(parser->error, "column name is missing\n");
                return 1;
            }

            printf("Column name = %s\n", parser->lexer->value);
            columnsCount++;
            accept(parser, T_LIT_IDENTIFIER);
        } while (accept(parser, T_OP_COMMA));

        if (!accept(parser, T_OP_RPAREN)) {
            parser->hasError = 1;
            sprintf(parser->error, "right parenthesis missing\n");
            return 1;
        }
    }

    if (!accept(parser, T_KW_VALUES)) {
        parser->hasError = 1;
        sprintf(parser->error, "VALUES keyword expected\n");
        return 1;
    }

    // Values
    if (!accept(parser, T_OP_LPAREN)) {
        parser->hasError = 1;
        sprintf(parser->error, "left parenthesis missing after VALUES\n");
        return 1;
    }

    do {
        if (!is(parser, T_LIT_STR_DOUBLE_QUOTE) && !is(parser, T_LIT_STR_SIMPLE_QUOTE) && !is(parser, T_LIT_FLOAT) &&
            !is(parser, T_LIT_INT)) {
            parser->hasError = 1;
            sprintf(parser->error, "Value is missing (STRING|FLOAT|INT)\n");
            return 1;
        }

        printf("Value = %s", parser->lexer->value);
        valuesCount++;
        accept(parser, parser->lexer->tok);
    } while (accept(parser, T_OP_COMMA));

    if (valuesCount != columnsCount) {
        parser->hasError = 1;
        sprintf(parser->error, "Values count must be the same as columns count\n");
        return 1;
    }

    if (!accept(parser, T_OP_RPAREN)) {
        parser->hasError = 1;
        sprintf(parser->error, "right parenthesis missing\n");
        return 1;
    }

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting after %s\n", tableName);
        return 1;
    }

    printf("INSERT INTO %s\n", tableName);

    return 0;
}


int stmtUpdate(Parser *parser, Database **database) {
    char *tableName;

    if (!accept(parser, T_KW_UPDATE)) {
        return 1;
    }

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after UPDATE\n");
        return 1;
    }

    tableName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);

    if (!accept(parser, T_KW_SET)) {
        parser->hasError = 1;
        sprintf(parser->error, "SET keyword is missing after %s\n", tableName);
        return 1;
    }

    do {
        if (!is(parser, T_LIT_IDENTIFIER)) {
            parser->hasError = 1;
            sprintf(parser->error, "column name is missing\n");
            return 1;
        }

        printf("Column name = %s\n", parser->lexer->value);
        accept(parser, T_LIT_IDENTIFIER);

        if (!accept(parser, T_OP_EQUAL)) {
            parser->hasError = 1;
            // TODO: set column name here
            sprintf(parser->error, "'=' is missing after %s\n", "COLUMN NAME");
            return 1;
        }

        if (!is(parser, T_LIT_STR_DOUBLE_QUOTE) && !is(parser, T_LIT_STR_SIMPLE_QUOTE) && !is(parser, T_LIT_FLOAT) &&
            !is(parser, T_LIT_INT)) {
            parser->hasError = 1;
            sprintf(parser->error, "Value is missing (STRING|FLOAT|INT)\n");
            return 1;
        }

        printf("Value = %s\n", parser->lexer->value);
        accept(parser, parser->lexer->tok);
    } while (accept(parser, T_OP_COMMA));


    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting\n");
        return 1;
    }

    // TODO: WHERE CLAUSE

    printf("UPDATE %s\n", tableName);

    return 0;
}

int stmtDelete(Parser *parser, Database **database) {
    char *tableName;

    if (!accept(parser, T_KW_DELETE)) {
        return 1;
    }

    if (!accept(parser, T_KW_FROM)) {
        parser->hasError = 1;
        sprintf(parser->error, "FROM keyword is missing after DELETE\n");
        return 1;
    }

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after DROP FROM\n");
        return 1;
    }

    tableName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);

    // TODO: WHERE CLAUSE

    if (!accept(parser, T_OP_SEMICOLON)) {
        parser->hasError = 1;
        sprintf(parser->error, "';' expecting\n");
        return 1;
    }

    printf("DELETE FROM %s\n", tableName);

    return 0;
}