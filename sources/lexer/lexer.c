/*
**  Filename: lexer.c
**
**  Made by: Walkoss on 18/11/2017.
**
**  Description: 
*/

#include "lexer.h"
#include "file/file.h"

void lexerMakeTokenLiteral(Lexer *lexer, char *keyword, TokenType type) {
    TokenHash *tokenHash;

    tokenHash = hashMapGet(lexer->keywordsHashTable, keyword);
    if (tokenHash == NULL) {
        tokenHash = xmalloc(sizeof(TokenHash), __func__);
        if (tokenHash != NULL) {
            strcpy(tokenHash->value, keyword);
            tokenHash->token = type;
            hashMapPut(lexer->keywordsHashTable, keyword, tokenHash);
        }
    } else {
        lexer->tok = tokenHash->token;
        strcpy(lexer->value, tokenHash->value);
    }
}

void lexerInitKeywords(Lexer *lexer) {
    lexerMakeTokenLiteral(lexer, "create", T_KW_CREATE);
    lexerMakeTokenLiteral(lexer, "drop", T_KW_DROP);
    lexerMakeTokenLiteral(lexer, "use", T_KW_USE);
    lexerMakeTokenLiteral(lexer, "database", T_KW_DATABASE);
    lexerMakeTokenLiteral(lexer, "table", T_KW_TABLE);
}

Lexer *lexerInit(char *buffer) {
    Lexer *lexer;

    lexer = xmalloc(sizeof(Lexer), __func__);
    if (lexer != NULL) {
        lexer->error = xmalloc(MAX_LEXICAL_VALUE, __func__);
        lexer->cur = T_EOS;
        lexer->lineNb = 0;
        lexer->curPos = 1;
        lexer->tok = T_EOS;
        lexer->value = xmalloc(MAX_LEXICAL_VALUE, __func__);
        lexer->buffer = buffer;
        lexer->cursor = 0;
        lexer->bufferSize = strlen(buffer);
        lexer->keywordsHashTable = hashMapInit(NUM_KW_TOKENS);
        lexerInitKeywords(lexer);
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
        hashMapFree(lexer->keywordsHashTable);
        free(lexer);
    }
}

int lexerNextCharacter(Lexer *lexer) {
    if (lexer->cursor < lexer->bufferSize) {
        lexer->cur = lexer->buffer[lexer->cursor++];
        if (lexer->cur == '\n') {
            lexer->lineNb++;
            lexer->curPos = 0;
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

void lexerSetError(Lexer *lexer, char *msg) {
    lexer->error = strcpy(lexer->error, msg);
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
    lexerNextCharacter(lexer);
    while (!lexerIsEos(lexer) && quote != (character = lexerNextCharacter
            (lexer))) {
        // TODO: handle \t, ... + check if lineNb is incremented when \n
        if (character == '\\') {
            character = lexerNextCharacter(lexer);
            if (character == 'n') {
                character = '\n';
                lexer->lineNb++;
                lexer->curPos = 0;
            }
        }
        buffer[len++] = (char) character;
    }

    if (lexerIsEos(lexer) && lexer->cur != quote) {
        // TODO: set error QUOTE END
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
           || isalpha(character)) {
        lexerNextCharacter(lexer);
        buffer[len++] = (char) character;
    }

    buffer[len] = '\0';
    lexer->value = strdup(buffer);

    bufferToLower = strdup(buffer);
    strToLower(bufferToLower);
    tokenType = (TokenType *) hashMapGet(lexer->keywordsHashTable,
                                         bufferToLower);
    if (tokenType) {
        return *tokenType;
    }

    return T_LIT_IDENTIFIER;
}

TokenType getToken(Lexer *lexer) {
    int currentCharacter;

    while (!lexerIsEos(lexer) && isspace(lexerPeekCharacter(lexer))) {
        lexerNextCharacter(lexer);
    }
    if (lexerIsEos(lexer)) {
        return lexerSetTokenType(lexer, T_EOS);
    }

    currentCharacter = lexerPeekCharacter(lexer);
    printf("\nCurrent character: %c\n", currentCharacter);

    // TODO: refactor this using function pointers maybe?
    if (currentCharacter == '\'' || currentCharacter == '"' ||
        currentCharacter == '`') {
        return lexerSetTokenType(
                lexer,
                readLiteralString(lexer, currentCharacter)
        );
    } else if (isalpha(currentCharacter) || currentCharacter == '_') {
        return lexerSetTokenType(lexer, readIdentifierOrKeyword(lexer));
    }
    return lexerSetTokenType(lexer, T_ILLEGAL);
}