#include "parser/parser.h"

int main(int argc, char **argv) {
    Lexer *lexer;
    TokenType tok;

    if (argc >= 2) {
        lexer = lexerInit(argv[1]);
        while ((tok = getToken(lexer)) != T_EOS && tok != T_ILLEGAL) {
            printf("TokenType: %s\n", tokenTypeAsString(tok));
            tokenInspect(lexer);
        }
        if (tok == T_ILLEGAL) {
            lexerDisplayError(lexer);
        }
        lexerFree(lexer);
    }
    return 0;
}