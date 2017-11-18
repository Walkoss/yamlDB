/*
**  Filename: lexer.c
**
**  Made by: Walkoss on 18/11/2017.
**
**  Description: 
*/

#include "lexer.h"

void lexerInit(Lexer *lexer, FILE *stream) {
    lexer->stream = stream;
    lexer->error = NULL;
    lexer->linenumber = 1;
}

int next(Lexer *lexer) {
    lexer->stash = fgetc(lexer->stream);
    return lexer->stash;
}

void undo(Lexer *lexer) {
    ungetc(lexer->stash, lexer->stream);
}

int token(Lexer *lexer, TokenType tokenType) {
    lexer->token.type = tokenType;
    return lexer->token.type;
}

int scanString(Lexer *lexer, int quote) {
    int len;
    int currentCharacter;
    char buffer[BUFFER_SIZE];

    len = 0;
    // TODO: handle quote end "test' for example
    while (quote != (currentCharacter = next(lexer))) {
        switch (currentCharacter) {
            case '\n':
                lexer->linenumber++;
                break;
            case '\\':
                switch (currentCharacter = next(lexer)) {
                    case 'n':
                        currentCharacter = '\n';
                        break;
                    case 't':
                        currentCharacter = '\t';
                        break;
                }
                break;
        }
        buffer[len++] = currentCharacter;
    }

    buffer[len++] = '\0';
    lexer->token.len = len;
    lexer->token.value.asString = strdup(buffer);

    switch (quote) {
        case '\'':
            return token(lexer, STRING_SIMPLE_QUOTE);
        case '"':
            return token(lexer, STRING_DOUBLE_QUOTE);
        case '`':
            return token(lexer, STRING_BACK_QUOTE);
    }
    return token(lexer, ILLEGAL);
}

int scanIdentifier(Lexer *lexer) {
    int len;
    int currentCharacter;
    char buffer[BUFFER_SIZE];

    len = 0;
    currentCharacter = lexer->stash;
    while (isalpha(currentCharacter) || currentCharacter == '_') {
        buffer[len++] = currentCharacter;
        currentCharacter = next(lexer);
    }

    undo(lexer);
    buffer[len++] = '\0';
    lexer->token.len = len;
    lexer->token.value.asString = strdup(buffer);

    // TODO: scan keywords (DATABASE, CREATE, etc..)

    return token(lexer, IDENTIFIER);
}

int scanInt(Lexer *lexer) {
    // TODO: scan int and float
    lexer->token.value.asInt = 4;

    return token(lexer, INT);
}

int lexerNext(Lexer *lexer) {
    int currentCharacter;

    currentCharacter = next(lexer);
    if (isspace(currentCharacter)) {
        return lexerNext(lexer);
    }
    // TODO: refactor this using function pointers maybe?
    switch (currentCharacter) {
        case '(':
            return token(lexer, LPAREN);
        case ')':
            return token(lexer, RPAREN);
        case ';':
            return token(lexer, SEMICOLON);
        case ',':
            return token(lexer, COMMA);
        case '=':
            if (next(lexer) != '=') {
                undo(lexer);
                return token(lexer, OP_ASSIGN);
            }
            return token(lexer, OP_EQ);
        case '\n':
            lexer->linenumber++;
            return token(lexer, NEWLINE);
        case '\'':
        case '"':
        case '`':
            return scanString(lexer, currentCharacter);
        case EOF: // TODO: test it
            token(lexer, EOS);
            return 0;
        default:
            if (isalpha(currentCharacter) || currentCharacter == '_') {
                return scanIdentifier(lexer);
            } else if (isdigit(currentCharacter) || currentCharacter == '.') {
                return scanInt(lexer);
            }
            lexer->error = xmalloc(
                    sizeof(char) * strlen("Illegal character 'c'" + 1),
                    __func__);
            sprintf(lexer->error, "Illegal character '%c'", currentCharacter);
            token(lexer, ILLEGAL);
            return 0;
    }
}