#include "parser/parser.h"

int main(int argc, char **argv) {
    Parser *parser;

    if (argc >= 2) {
        parser = parserInit(argv[1]);
        parse(parser);
        parserFree(parser);
    }
    return 0;
}