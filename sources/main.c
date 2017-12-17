#include "parser/parser.h"

int main(int argc, char **argv) {
    Parser *parser;
    char *text;

    printf("\033c");
    while (1) {
        printf("Entrez votre requête SQL\n");
        text = getUserInput(9999);
        if (!strcmp(text, "quit")) {
            free(text);
            break;
        }
        else if (text[0] != '\0') {
            parser = parserInit(text);
            parse(parser);
            parserFree(parser);
        }
        free(text);
    }

    return 0;
}