/*
**  Filename: statement.c
**
**  Made by: Walkoss on 10/12/2017.
**
**  Description:
*/

#include "statement.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"

/**
 * Use Database Statement ("USE databaseName;")
 * @param parser
 */
int stmtUseDatabase(Parser *parser, Database **database) {
    char *databaseName;

    if (!accept(parser, T_KW_USE)) {
        return -1;
    }

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "database name is missing after USE keyword");
        return 1;
    }

    databaseName = strdup(parser->lexer->value);
    accept(parser, T_LIT_IDENTIFIER);

    accept(parser, T_OP_SEMICOLON);

    if (*database == NULL || strcmp((*database)->name, databaseName) != 0)
        *database = initDatabase(databaseName);

    return useDatabase(*database);
}


/**
 * Create [Database|Table] Statement
 * @param parser
 */
int stmtCreate(Parser *parser, Database **database) {
    if (!accept(parser, T_KW_CREATE)) {
        return -1;
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
 * Create Database Statement ("CREATE DATABASE databaseName;")
 * @param parser
 */
int stmtCreateDatabase(Parser *parser, Database **database) {
    char *databaseName;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "database name is missing after CREATE DATABASE");
        return 1;
    }

    databaseName = strdup(parser->lexer->value);
    accept(parser, T_LIT_IDENTIFIER);

    accept(parser, T_OP_SEMICOLON);

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
    int createTableResult;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after CREATE TABLE");
        return 1;
    }

    accept(parser, T_LIT_IDENTIFIER);
    table = xmalloc(sizeof(Table), __func__);
    table->name = strdup(parser->lexer->value);
    table->fieldHead = NULL;

    if (!accept(parser, T_OP_LPAREN)) {
        parser->hasError = 1;
        sprintf(parser->error, "left parenthesis missing after %s", table->name);
        return 1;
    }

    do {
        if (!is(parser, T_LIT_IDENTIFIER)) {
            parser->hasError = 1;
            sprintf(parser->error, "field name is missing");
            return 1;
        }

        fieldName = strdup(parser->lexer->value);
        accept(parser, T_LIT_IDENTIFIER);

        if (!is(parser, T_KW_INT) && !is(parser, T_KW_FLOAT) && !is(parser, T_KW_VARCHAR) && !is(parser, T_KW_CHAR)) {
            parser->hasError = 1;
            sprintf(parser->error, "field type is missing (INT|FLOAT|CHAR|VARCHAR)");
            return 1;
        }

        switch (parser->lexer->tok) {
            case T_KW_INT:
                fieldType = INT;
                break;
            case T_KW_FLOAT:
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
        sprintf(parser->error, "right parenthesis missing");
        return 1;
    }

    accept(parser, T_OP_SEMICOLON);

    createTableResult = createTable(*database, table);
    if (createTableResult == 0) {
        initTables(*database);
    }
    return createTableResult;
}


/**
 * Drop [Database|Table] Statement
 * @param parser
 */
int stmtDrop(Parser *parser, Database **database) {
    if (!accept(parser, T_KW_DROP)) {
        return -1;
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
 * DROP DATABASE Statement ("DROP DATABASE databaseName;")
 * @param parser
 */
int stmtDropDatabase(Parser *parser, Database **database) {
    char *databaseName;

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "database name is missing after DROP DATABASE");
        return 1;
    }

    databaseName = strdup(parser->lexer->value);
    accept(parser, T_LIT_IDENTIFIER);

    accept(parser, T_OP_SEMICOLON);

    if (*database == NULL || strcmp((*database)->name, databaseName) != 0)
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
        sprintf(parser->error, "table name is missing after DROP TABLE");
        return 1;
    }

    table = xmalloc(sizeof(Table), __func__);
    table->name = strdup(parser->lexer->value);
    accept(parser, T_LIT_IDENTIFIER);

    accept(parser, T_OP_SEMICOLON);

    return dropTable(*database, table);
}

int stmtInsert(Parser *parser, Database **database) {
    char *tableName;
    Data *dataHead;
    Table *table;
    Field *fieldHead;
    Data *data;

    dataHead = NULL;

    if (!accept(parser, T_KW_INSERT)) {
        return -1;
    }

    if (!accept(parser, T_KW_INTO)) {
        parser->hasError = 1;
        sprintf(parser->error, "INTO keyword is missing after INSERT");
        return 1;
    }

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after INSERT INTO");
        return 1;
    }

    tableName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);
    table = findTable(*database, tableName);

    if (!table) {
        return 1;
    }

    fieldHead = table->fieldHead;

    if (!accept(parser, T_KW_VALUES)) {
        parser->hasError = 1;
        sprintf(parser->error, "VALUES keyword expected");
        return 1;
    }

    // Values
    if (!accept(parser, T_OP_LPAREN)) {
        parser->hasError = 1;
        sprintf(parser->error, "left parenthesis missing after VALUES");
        return 1;
    }

    do {
        if (!is(parser, T_LIT_STR_DOUBLE_QUOTE) && !is(parser, T_LIT_STR_SIMPLE_QUOTE) && !is(parser, T_LIT_FLOAT) &&
            !is(parser, T_LIT_INT)) {
            parser->hasError = 1;
            sprintf(parser->error, "Value is missing (STRING|FLOAT|INT)");
            return 1;
        }

        if (fieldHead == NULL) {
            parser->hasError = 1;
            sprintf(parser->error, "More data than expected");
            return 1;
        }

        data = xmalloc(sizeof(Data), __func__);

        if (data) {
            data->value = strdup(parser->lexer->value);
            data->field = fieldHead;
            data->next = NULL;

            if ((data->field->type == INT && parser->lexer->tok != T_LIT_INT) ||
                (data->field->type == FLOAT && parser->lexer->tok != T_LIT_FLOAT) ||
                (data->field->type == CHAR && parser->lexer->tok != T_LIT_STR_SIMPLE_QUOTE) ||
                (data->field->type == CHAR && strlen(data->value) > 1) ||
                (data->field->type == VARCHAR &&
                 (parser->lexer->tok != T_LIT_STR_SIMPLE_QUOTE && parser->lexer->tok != T_LIT_STR_DOUBLE_QUOTE))) {
                parser->hasError = 1;
                sprintf(parser->error, "Value %s for field %s is incorrect (wrong data type)", data->value,
                        fieldHead->name);
                return 1;
            }
        }

        dataListAppend(&dataHead, data);
        fieldHead = fieldHead->next;
        accept(parser, parser->lexer->tok);
    } while (accept(parser, T_OP_COMMA));

    if (fieldHead != NULL) {
        parser->hasError = 1;
        sprintf(parser->error, "Value for field %s is missing", fieldHead->name);
        return 1;
    }

    if (!accept(parser, T_OP_RPAREN)) {
        parser->hasError = 1;
        sprintf(parser->error, "right parenthesis missing");
        return 1;
    }

    accept(parser, T_OP_SEMICOLON);

    return addData(*database, table, dataHead);
}


int stmtUpdate(Parser *parser, Database **database) {
    char *tableName;
    Table *table;
    Data *data;
    Condition *condition;

    condition = NULL;
    if (!accept(parser, T_KW_UPDATE)) {
        return -1;
    }

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after UPDATE");
        return 1;
    }

    tableName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);
    table = findTable(*database, tableName);

    if (!table) {
        return 1;
    }

    if (!accept(parser, T_KW_SET)) {
        parser->hasError = 1;
        sprintf(parser->error, "SET keyword is missing");
        return 1;
    }

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "colum name is missing");
        return 1;
    }

    data = xmalloc(sizeof(Data), __func__);
    if (!data)
        return 1;
    data->field = findField(table, parser->lexer->value);
    if (data->field == NULL) {
        parser->hasError = 1;
        sprintf(parser->error, "field name %s doesn't exist", parser->lexer->value);
        return 1;
    }
    accept(parser, parser->lexer->tok);

    if (!accept(parser, T_OP_EQUAL)) {
        parser->hasError = 1;
        sprintf(parser->error, "'=' is expected after %s", data->field->name);
        return 1;
    }

    if (!is(parser, T_LIT_INT) && !is(parser, T_LIT_FLOAT) && !is(parser, T_LIT_STR_DOUBLE_QUOTE) &&
        !is(parser, T_LIT_STR_SIMPLE_QUOTE)) {
        parser->hasError = 1;
        sprintf(parser->error, "value is missing after '='");
        return 1;
    }

    if ((data->field->type == INT && parser->lexer->tok != T_LIT_INT) ||
        (data->field->type == FLOAT && parser->lexer->tok != T_LIT_FLOAT) ||
        (data->field->type == CHAR && parser->lexer->tok != T_LIT_STR_SIMPLE_QUOTE) ||
        (data->field->type == CHAR && strlen(data->value) > 1) ||
        (data->field->type == VARCHAR &&
         (parser->lexer->tok != T_LIT_STR_SIMPLE_QUOTE && parser->lexer->tok != T_LIT_STR_DOUBLE_QUOTE))) {
        parser->hasError = 1;
        sprintf(parser->error, "Value %s for field %s is incorrect (wrong data type)", parser->lexer->value,
                data->field->name);
        return 1;
    }

    data->value = strdup(parser->lexer->value);
    accept(parser, parser->lexer->tok);

    if (accept(parser, T_KW_WHERE)) {
        if (!is(parser, T_LIT_IDENTIFIER)) {
            parser->hasError = 1;
            sprintf(parser->error, "column name is missing");
            return 1;
        }

        condition = xmalloc(sizeof(Condition), __func__);
        if (!condition)
            return 1;
        condition->key = strdup(parser->lexer->value);
        accept(parser, T_LIT_IDENTIFIER);

        if (!accept(parser, T_OP_EQUAL)) {
            parser->hasError = 1;
            sprintf(parser->error, "'=' is missing after %s", condition->key);
            return 1;
        }

        if (!is(parser, T_LIT_STR_DOUBLE_QUOTE) && !is(parser, T_LIT_STR_SIMPLE_QUOTE) && !is(parser, T_LIT_FLOAT) &&
            !is(parser, T_LIT_INT)) {
            parser->hasError = 1;
            sprintf(parser->error, "Value is missing (STRING|FLOAT|INT)");
            return 1;
        }

        condition->value = strdup(parser->lexer->value);
        accept(parser, parser->lexer->tok);
    }

    accept(parser, T_OP_SEMICOLON);

    return openFilesForUpdating(*database, table, data, condition);
}

int stmtDelete(Parser *parser, Database **database) {
    char *tableName;
    Table *table;
    Condition *condition;
    Field *field;

    condition = NULL;
    if (!accept(parser, T_KW_DELETE)) {
        return -1;
    }

    if (!accept(parser, T_KW_FROM)) {
        parser->hasError = 1;
        sprintf(parser->error, "FROM keyword is missing after DELETE");
        return 1;
    }

    if (!is(parser, T_LIT_IDENTIFIER)) {
        parser->hasError = 1;
        sprintf(parser->error, "table name is missing after DROP FROM");
        return 1;
    }

    tableName = parser->lexer->value;
    accept(parser, T_LIT_IDENTIFIER);
    table = findTable(*database, tableName);

    if (!table) {
        return 1;
    }

    if (accept(parser, T_KW_WHERE)) {
        if (!is(parser, T_LIT_IDENTIFIER)) {
            parser->hasError = 1;
            sprintf(parser->error, "column name is missing");
            return 1;
        }

        condition = xmalloc(sizeof(Condition), __func__);
        if (!condition)
            return 1;
        condition->key = strdup(parser->lexer->value);
        field = findField(table, parser->lexer->value);
        if (field == NULL) {
            parser->hasError = 1;
            sprintf(parser->error, "field name %s doesn't exist", parser->lexer->value);
            return 1;
        }
        accept(parser, T_LIT_IDENTIFIER);

        if (!accept(parser, T_OP_EQUAL)) {
            parser->hasError = 1;
            sprintf(parser->error, "'=' is missing after %s", condition->key);
            return 1;
        }
        accept(parser, T_OP_EQUAL);

        if (!is(parser, T_LIT_STR_DOUBLE_QUOTE) && !is(parser, T_LIT_STR_SIMPLE_QUOTE) && !is(parser, T_LIT_FLOAT) &&
            !is(parser, T_LIT_INT)) {
            parser->hasError = 1;
            sprintf(parser->error, "Value is missing (STRING|FLOAT|INT)");
            return 1;
        }

        condition->value = strdup(parser->lexer->value);

        if ((field->type == INT && parser->lexer->tok != T_LIT_INT) ||
            (field->type == FLOAT && parser->lexer->tok != T_LIT_FLOAT) ||
            (field->type == CHAR && parser->lexer->tok != T_LIT_STR_SIMPLE_QUOTE) ||
            (field->type == CHAR && strlen(condition->value) > 1) ||
            (field->type == VARCHAR &&
             (parser->lexer->tok != T_LIT_STR_SIMPLE_QUOTE && parser->lexer->tok != T_LIT_STR_DOUBLE_QUOTE))) {
            parser->hasError = 1;
            sprintf(parser->error, "Value %s for field %s is incorrect (wrong data type)", parser->lexer->value,
                    field->name);
            return 1;
        }

        accept(parser, parser->lexer->tok);
    }

    accept(parser, T_OP_SEMICOLON);

    return openFilesForRemoving(*database, table, condition);
}