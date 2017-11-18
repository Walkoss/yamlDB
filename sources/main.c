#include <stdio.h>
#include <stdlib.h>
#include "lexer/lexer.h"

int main() {
    Lexer *lexer;

    lexer = malloc(sizeof(lexer));
    lexerInit(lexer, stdin);

    while (lexerNext(lexer)) {
        tokenInspect(lexer->token);
    }

    if (lexer->token.type == ILLEGAL) {
        fprintf(stderr, "SyntaxError %s on line %d", lexer->error,
                lexer->linenumber);
    }
    return 0;
}