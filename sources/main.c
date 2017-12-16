#include "parser/parser.h"
#include "interface/interface.h"
#include "gtk/gtk.h"

int main(int argc, char **argv) {
    /*Parser *parser;

    if (argc >= 2) {
        parser = parserInit(argv[1]);
        parse(parser);
        parserFree(parser);
    }*/
    run(argc, argv);
    return 0;
}