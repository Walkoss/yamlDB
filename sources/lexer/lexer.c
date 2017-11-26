/*
**  Filename: lexer.c
**
**  Made by: Walkoss on 18/11/2017.
**
**  Description: 
*/

#include "lexer.h"

void lexerMakeTokenLiteral(Lexer *lexer, char *keyword, TokenType type) {
    TokenHash *tokenHash;

    tokenHash = hashMapGet(lexer->tokensHashTable, keyword);
    if (tokenHash == NULL) {
        tokenHash = xmalloc(sizeof(TokenHash), __func__);
        if (tokenHash != NULL) {
            strcpy(tokenHash->value, keyword);
            tokenHash->token = type;
            hashMapPut(lexer->tokensHashTable, keyword, tokenHash);
        }
    } else {
        lexer->tok = tokenHash->token;
        strcpy(lexer->value, tokenHash->value);
    }
}

void lexerInitTokensHashTable(Lexer *lexer) {
    // Operators
    lexerMakeTokenLiteral(lexer, "(", T_OP_LPAREN);
    lexerMakeTokenLiteral(lexer, ")", T_OP_RPAREN);
    lexerMakeTokenLiteral(lexer, ";", T_OP_SEMICOLON);
    lexerMakeTokenLiteral(lexer, ",", T_OP_COMMA);
    lexerMakeTokenLiteral(lexer, "=", T_OP_EQUAL);

    // Keywords
    lexerMakeTokenLiteral(lexer, "create", T_KW_CREATE);
    lexerMakeTokenLiteral(lexer, "drop", T_KW_DROP);
    lexerMakeTokenLiteral(lexer, "use", T_KW_USE);
    lexerMakeTokenLiteral(lexer, "database", T_KW_DATABASE);
    lexerMakeTokenLiteral(lexer, "table", T_KW_TABLE);
    lexerMakeTokenLiteral(lexer, "int", T_KW_INT);
    lexerMakeTokenLiteral(lexer, "float", T_KW_FLOAT);
    lexerMakeTokenLiteral(lexer, "varchar", T_KW_VARCHAR);
}

Lexer *lexerInit(char *buffer) {
    Lexer *lexer;

    lexer = xmalloc(sizeof(Lexer), __func__);
    if (lexer != NULL) {
        lexer->error = xmalloc(MAX_LEXICAL_VALUE, __func__);
        lexer->cur = T_EOS;
        lexer->lineNb = 1;
        lexer->curPos = 1;
        lexer->tok = T_EOS;
        lexer->value = xmalloc(MAX_LEXICAL_VALUE, __func__);
        lexer->buffer = buffer;
        lexer->cursor = 0;
        lexer->bufferSize = strlen(buffer);
        lexer->tokensHashTable = hashMapInit(NUM_OP_KW_TOKENS);
        lexerInitTokensHashTable(lexer);
    }

    return lexer;
}

void lexerFree(Lexer *lexer) {
    if (lexer != NULL) {
        if (lexer->value != NULL) {
            free(lexer->value);
        }
        if (lexer->error != NULL) {
            free(lexer->error);
        }
        hashMapFree(lexer->tokensHashTable);
        free(lexer);
    }
}

int lexerNextCharacter(Lexer *lexer) {
    if (lexer->cursor < lexer->bufferSize) {
        lexer->cur = lexer->buffer[lexer->cursor++];
        if (lexer->cur == '\n') {
            lexer->lineNb++;
            lexer->curPos = 1;
        } else {
            lexer->curPos++;
        }
    } else {
        lexer->cur = T_EOS;
    }
    return lexer->cur;
}

int lexerPeekCharacter(Lexer *lexer) {
    int character;

    if (lexer->cursor < lexer->bufferSize) {
        character = lexer->buffer[lexer->cursor];
    } else {
        character = T_EOS;
    }
    return character;
}

int lexerIsEos(Lexer *lexer) {
    return lexerPeekCharacter(lexer) == T_EOS;
}

void lexerDisplayError(Lexer *lexer) {
    fprintf(stderr, "SyntaxError: %s at line %zd and position %zd",
            lexer->error, lexer->lineNb, lexer->curPos);
}

void tokenInspect(Lexer *lexer) {
    switch (lexer->tok) {
        case T_LIT_IDENTIFIER:
            printf("id: %s\n", lexer->value);
            break;
        case T_LIT_STR_DOUBLE_QUOTE:
        case T_LIT_STR_SIMPLE_QUOTE:
        case T_LIT_STR_BACK_QUOTE:
            printf("string: %s\n", lexer->value);
            break;
        case T_LIT_INT:
            printf("int: %d\n", atoi(lexer->value));
            break;
        case T_LIT_FLOAT:
            printf("int: %f\n", atof(lexer->value));
            break;
        default:
            printf("type: %s\n", tokenTypeAsString(lexer->tok));
    }
}

TokenType lexerSetTokenType(Lexer *lexer, TokenType tokenType) {
    lexer->tok = tokenType;
    return lexer->tok;
}

TokenType readLiteralString(Lexer *lexer, int quote) {
    int character;
    int len;
    char buffer[MAX_LEXICAL_VALUE];

    len = 0;
    lexerNextCharacter(lexer); // To consume the first quote
    while (!lexerIsEos(lexer) && quote != (character = lexerNextCharacter
            (lexer))) {
        // TODO: handle \t, ... + check if lineNb is incremented when \n
        if (character == '\\') {
            character = lexerNextCharacter(lexer);
            if (character == 'n') {
                character = '\n';
                lexer->lineNb++;
                lexer->curPos = 1;
            }
        }
        buffer[len++] = (char) character;
    }

    if (lexerIsEos(lexer) && lexer->cur != quote) {
        sprintf(lexer->error, "Missing end quote (%c)", quote);
        return T_ILLEGAL;
    }

    buffer[len] = '\0';
    lexer->value = strdup(buffer);
    if (quote == '\'')
        return T_LIT_STR_SIMPLE_QUOTE;
    else if (quote == '`')
        return T_LIT_STR_BACK_QUOTE;
    return T_LIT_STR_DOUBLE_QUOTE;
}

TokenType readIdentifierOrKeyword(Lexer *lexer) {
    int character;
    int len;
    char buffer[MAX_LEXICAL_VALUE];
    TokenType *tokenType;
    char *bufferToLower;

    len = 0;
    while ((character = lexerPeekCharacter(lexer)) == '_'
           || isalpha(character) || isdigit(character)) {
        lexerNextCharacter(lexer);
        buffer[len++] = (char) character;
    }

    buffer[len] = '\0';
    lexer->value = strdup(buffer);

    bufferToLower = strdup(buffer);
    strToLower(bufferToLower);
    tokenType = (TokenType *) hashMapGet(lexer->tokensHashTable,
                                         bufferToLower);
    if (tokenType) {
        return *tokenType;
    }

    return T_LIT_IDENTIFIER;
}

TokenType readNumber(Lexer *lexer) {
    int character;
    int len;
    char buffer[MAX_LEXICAL_VALUE];
    int hasDot;
    TokenType tokenType;

    tokenType = T_LIT_INT;
    len = 0;
    hasDot = 0;
    while ((character = lexerPeekCharacter(lexer)) == '.'
           || isdigit(character)) {
        if (character == '.') {
            if (hasDot) {
                sprintf(lexer->error, "multiple dot for float number");
                return T_ILLEGAL;
            } else {
                hasDot = 1;
                tokenType = T_LIT_FLOAT;
            }
        }
        buffer[len++] = (char) character;
        lexerNextCharacter(lexer);
    }

    buffer[len] = '\0';
    lexer->value = strdup(buffer);

    return tokenType;
}

int lexerIsOperator(Lexer *lexer) {
    char operator[2];
    int currentCharacter;
    HashMapNode *node;

    currentCharacter = lexerPeekCharacter(lexer);
    operator[0] = (char) currentCharacter;
    operator[1] = '\0';
    node = hashMapGet(lexer->tokensHashTable, operator);
    if (node == NULL) {
        return 0;
    }
    return 1;
}

TokenType readOperator(Lexer *lexer) {
    char operator[2];
    TokenType *tokenType;

    lexerNextCharacter(lexer);
    operator[0] = (char) lexer->cur;
    operator[1] = '\0';
    tokenType = hashMapGet(lexer->tokensHashTable, operator);
    if (tokenType == NULL) {
        sprintf(lexer->error, "Unexpected operator '%c'", lexer->cur);
        return T_ILLEGAL;
    }
    return *tokenType;
}

TokenType getToken(Lexer *lexer) {
    int currentCharacter;

    while (!lexerIsEos(lexer) && isspace(lexerPeekCharacter(lexer))) {
        lexerNextCharacter(lexer);
    }
    if (lexerIsEos(lexer)) {
        return lexerSetTokenType(lexer, T_EOS);
    }

    printf("\n");
    currentCharacter = lexerPeekCharacter(lexer);

    // TODO: refactor this using function pointers maybe?
    if (currentCharacter == '\'' || currentCharacter == '"' ||
        currentCharacter == '`') {
        return lexerSetTokenType(
                lexer,
                readLiteralString(lexer, currentCharacter)
        );
    } else if (isalpha(currentCharacter) || currentCharacter == '_') {
        return lexerSetTokenType(lexer, readIdentifierOrKeyword(lexer));
    } else if (isdigit(currentCharacter) || currentCharacter == '.') {
        return lexerSetTokenType(lexer, readNumber(lexer));
    } else if (lexerIsOperator(lexer)) {
        return lexerSetTokenType(lexer, readOperator(lexer));
    }
    // TODO handle special char '\n \t'
    sprintf(lexer->error, "Unexpected character '%c'", currentCharacter);
    return lexerSetTokenType(lexer, T_ILLEGAL);
}