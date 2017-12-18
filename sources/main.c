#include "parser/parser.h"
#include "print_color/print_color.h"

int main(int argc, char **argv) {
    Database *database;
    Parser *parser;
    char *sqlQuery;
    int parseResult;
    int stop;

    stop = 0;
    print_color(COLOR_YELLOW, "Welcome to YamlDB [Part 3]\n");
    print_color(COLOR_YELLOW, "Write your SQL query\n");
    while (stop == 0) {
        print_color(COLOR_YELLOW, "\n~> ");
        sqlQuery = getUserInput();
        if (sqlQuery[0] == '\0') {
            print_error_color("Please enter a SQL Query\n");
        } else if (!strcmp(sqlQuery, "quit")) {
            print_color(COLOR_YELLOW, "\nSee you next time, hope you enjoyed our database :)\n");
            stop = 1;
        } else {
            parser = parserInit(sqlQuery);
            parseResult = parse(parser, &database);
            if (parseResult != 0) {
                parserFree(parser);
                return parseResult;
            }
            parserFree(parser);
        }
        free(sqlQuery);
    }

    return 0;
}